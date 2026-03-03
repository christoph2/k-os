#include "Generator.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <functional>
#include <string>
#include <optional>
#include <sstream>
#include <regex>
#include <set>
#include <unordered_map>
#include <vector>

namespace kos::oilcpp {

namespace {

std::string sanitize(const std::string& name) {
    std::string out;
    out.reserve(name.size() + 1);
    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '_') {
            out.push_back(c);
        } else {
            out.push_back('_');
        }
    }
    if (!out.empty() && std::isdigit(static_cast<unsigned char>(out.front()))) {
        out.insert(out.begin(), '_');
    }
    return out;
}

std::string toUpper(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return text;
}

std::string toLower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return text;
}

std::string stackSizeMacro(const std::string& taskName) {
    return toUpper(sanitize(taskName)) + "_STACK_SIZE";
}

std::vector<const ObjectInstance*> filter(const OilModel& model, const std::string& type) {
    std::vector<const ObjectInstance*> out;
    for (const auto& obj : model.application_objects) {
        if (obj.object_type == type) out.push_back(&obj);
    }
    return out;
}

std::optional<std::string> findAttr(const ObjectInstance& obj, const std::string& name) {
    for (auto it = obj.attributes.rbegin(); it != obj.attributes.rend(); ++it) {
        if (it->name == name) return it->value.text;
    }
    return std::nullopt;
}

std::optional<std::string> extractDriver(const std::optional<std::string>& typeAttr, const std::optional<std::string>& driverAttr) {
    if (driverAttr && !driverAttr->empty()) return driverAttr;
    if (typeAttr) {
        auto upper = toUpper(*typeAttr);
        auto pos = upper.find("DRIVER");
        if (pos != std::string::npos) {
            auto eq = upper.find('=', pos);
            if (eq != std::string::npos) {
                size_t idx = eq + 1;
                while (idx < typeAttr->size() && std::isspace(static_cast<unsigned char>((*typeAttr)[idx]))) ++idx;
                std::string name;
                while (idx < typeAttr->size()) {
                    char c = (*typeAttr)[idx];
                    if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') break;
                    name.push_back(c);
                    ++idx;
                }
                if (!name.empty()) return name;
            }
        }
    }
    return std::nullopt;
}

uint32_t toNumber(const std::optional<std::string>& val, uint32_t fallback = 0) {
    if (!val) return fallback;
    try {
        size_t idx = 0;
        auto v = *val;
        auto num = std::stoul(v, &idx, 0);
        if (idx == v.size()) return static_cast<uint32_t>(num);
    } catch (...) {
    }
    return fallback;
}

bool toBool(const std::optional<std::string>& val, bool fallback = false) {
    if (!val) return fallback;
    auto t = *val;
    // consider only leading token before whitespace/brace
    auto cut = t.find_first_of(" {;\t\r\n");
    if (cut != std::string::npos) t = t.substr(0, cut);
    std::transform(t.begin(), t.end(), t.begin(), ::toupper);
    if (t == "TRUE" || t == "STD_ON" || t == "YES" || t == "ON") return true;
    if (t == "FALSE" || t == "STD_OFF" || t == "NO" || t == "OFF") return false;
    return fallback;
}

std::vector<uint32_t> orderedPriorities(const std::vector<const ObjectInstance*>& tasks) {
    std::vector<uint32_t> prios;
    auto collect = [&prios](const std::vector<const ObjectInstance*>& list) {
        for (const auto* t : list) {
            auto prio = toNumber(findAttr(*t, "PRIORITY"), 0);
            if (std::find(prios.begin(), prios.end(), prio) == prios.end()) {
                prios.push_back(prio);
            }
        }
    };

    std::vector<const ObjectInstance*> autostart;
    std::vector<const ObjectInstance*> rest;
    for (const auto* t : tasks) {
        if (toBool(findAttr(*t, "AUTOSTART"), false)) {
            autostart.push_back(t);
        } else {
            rest.push_back(t);
        }
    }
    collect(autostart);
    collect(rest);
    return prios;
}

uint32_t legacyPriorityValue(size_t index) {
    return static_cast<uint32_t>((~static_cast<int>(index) + 1) & 0x0f);
}

void emitStringArray(std::ofstream& out, const char* name, const std::vector<const ObjectInstance*>& objs) {
    out << "const char* const " << name << "[" << objs.size() << "] = {";
    for (size_t i = 0; i < objs.size(); ++i) {
        out << "\"" << objs[i]->name << "\"";
        if (i + 1 < objs.size()) out << ", ";
    }
    out << "};\n";
}

std::unordered_map<std::string, size_t> indexByName(const std::vector<const ObjectInstance*>& objs) {
    std::unordered_map<std::string, size_t> map;
    for (size_t i = 0; i < objs.size(); ++i) {
        map[objs[i]->name] = i;
    }
    return map;
}

std::string parseActionType(const std::optional<std::string>& val) {
    if (!val) return "UNKNOWN";
    auto raw = *val;
    std::istringstream iss(raw);
    std::string first;
    iss >> first;
    return first.empty() ? std::string("UNKNOWN") : first;
}

std::string normalizeActionName(const std::string& raw, const std::vector<std::pair<std::string, int>>& serviceIds) {
    auto upper = toUpper(raw);
    for (const auto& svc : serviceIds) {
        if (toUpper(svc.first) == upper) return svc.first;
    }
    return raw;
}

std::optional<std::string> parseFirstToken(const std::optional<std::string>& val, const std::string& key) {
    if (!val) return std::nullopt;
    std::regex re(key + R"(\s*=\s*([A-Za-z_]\w*))", std::regex::icase);
    std::smatch m;
    if (std::regex_search(*val, m, re) && m.size() > 1) return m[1].str();
    return std::nullopt;
}

std::optional<uint32_t> parseNumberToken(const std::optional<std::string>& val, const std::string& key) {
    if (!val) return std::nullopt;
    std::regex re(key + R"(\s*=\s*([0-9]+))", std::regex::icase);
    std::smatch m;
    if (std::regex_search(*val, m, re) && m.size() > 1) {
        try {
            return static_cast<uint32_t>(std::stoul(m[1].str()));
        } catch (...) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

std::string firstTokenUpper(const std::optional<std::string>& val) {
    if (!val) return {};
    auto s = *val;
    auto end = s.find_first_of(" {");
    if (end != std::string::npos) s = s.substr(0, end);
    return toUpper(s);
}

std::optional<uint64_t> parseNumericToken(const std::optional<std::string>& val, const std::string& key) {
    if (!val) return std::nullopt;
    std::regex re(key + R"(\s*=\s*([0-9xa-fA-F]+))", std::regex::icase);
    std::smatch m;
    if (std::regex_search(*val, m, re) && m.size() > 1) {
        try {
            return static_cast<uint64_t>(std::stoull(m[1].str(), nullptr, 0));
        } catch (...) {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

std::string joinAppModes(const std::optional<std::string>& val) {
    if (!val) return {};
    std::regex re(R"(APPMODE\s*=\s*([A-Za-z_]\w*))", std::regex::icase);
    std::sregex_iterator it(val->begin(), val->end(), re);
    std::sregex_iterator end;
    std::vector<std::string> modes;
    for (; it != end; ++it) {
        if ((*it).size() > 1) modes.push_back((*it)[1].str());
    }
    if (modes.empty()) {
        auto trimmed = *val;
        trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char c) { return !std::isspace(c); }));
        trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char c) { return !std::isspace(c); }).base(), trimmed.end());
        std::string upper = trimmed;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        if (!trimmed.empty() && upper != "TRUE" && upper != "FALSE") modes.push_back(trimmed);
    }
    std::ostringstream oss;
    for (size_t i = 0; i < modes.size(); ++i) {
        oss << modes[i];
        if (i + 1 < modes.size()) oss << "|";
    }
    return oss.str();
}

struct MessageStats {
    size_t internal_count{0};
    size_t external_count{0};
};

MessageStats classifyMessages(const std::vector<const ObjectInstance*>& messages) {
    MessageStats stats;
    for (const auto* m : messages) {
        auto prop = findAttr(*m, "MESSAGEPROPERTY");
        if (prop && prop->find("INTERNAL") != std::string::npos) {
            ++stats.internal_count;
        } else {
            ++stats.external_count;
        }
    }
    return stats;
}

struct ConformanceInfo {
    std::string cc;
    bool hasMaxActivations;
};

ConformanceInfo computeConformance(const OilModel& model, const std::vector<const ObjectInstance*>& tasks) {
    auto os = filter(model, "OS");
    std::string cc = "AUTO";
    if (!os.empty()) {
        auto ccAttr = findAttr(*os.front(), "CC");
        if (ccAttr) cc = toUpper(*ccAttr);
    }
    bool hasExtendedTask = std::any_of(tasks.begin(), tasks.end(), [](const ObjectInstance* t) {
        return toUpper(findAttr(*t, "TYPE").value_or("BASIC")) == "EXTENDED";
    });
    bool hasMultiActivation = std::any_of(tasks.begin(), tasks.end(), [](const ObjectInstance* t) {
        return toNumber(findAttr(*t, "ACTIVATION"), 1) > 1;
    });
    if (cc == "AUTO") {
        if (hasExtendedTask) {
            cc = hasMultiActivation ? "ECC2" : "ECC1";
        } else {
            cc = hasMultiActivation ? "BCC2" : "BCC1";
        }
    }
    bool ccHasMaxActivations = (cc == "BCC2" || cc == "ECC2");
    return ConformanceInfo{cc, ccHasMaxActivations};
}

}  // namespace

void Generator::generate(const OilModel& model, const Options& opts) const {
    if (model.oil_version.empty()) {
        throw std::runtime_error("Model missing OIL version; parsing likely failed.");
    }

    if (opts.test_only) {
        return;  // validation-only run
    }

    auto out_pre = opts.output_prefix;
    out_pre.replace_extension(".pre.oil");
    if (!out_pre.parent_path().empty()) {
        std::error_code ec;
        std::filesystem::create_directories(out_pre.parent_path(), ec);
    }

    std::ofstream pre(out_pre, std::ios::binary);
    if (!pre) {
        throw std::runtime_error("Cannot open output file: " + out_pre.string());
    }
    pre << model.preprocessed_text;
    pre.close();

    auto out_info = opts.output_prefix;
    out_info.replace_extension(".info.txt");
    std::ofstream info(out_info, std::ios::binary);
    if (!info) {
        throw std::runtime_error("Cannot open output file: " + out_info.string());
    }
    info << "OIL_VERSION=" << model.oil_version << "\n";
    info << "Objects=" << model.application_objects.size() << "\n";
    if (opts.generate_orti) {
        info << "ORTI=requested\n";
    }
    if (!opts.target.empty()) {
        info << "TARGET=" << opts.target << "\n";
    }
    for (const auto& impl : model.implementation_specs) {
        info << "IMPLEMENTATION " << impl.object_type << "\n";
    }
    for (const auto& obj : model.application_objects) {
        info << obj.object_type << " " << obj.name << "\n";
        for (const auto& attr : obj.attributes) {
            info << "  " << attr.name << "=" << attr.value.text << "\n";
        }
    }
    info.close();

    writeOsCfgH(model, opts);
    writeOsCfgC(model, opts);
    if (opts.generate_orti) {
        writeOrti(model, opts);
    }
    writeMainStub(model, opts);
    writeTargetTemplate(opts);
}

void Generator::writeOsCfgH(const OilModel& model, const Options& opts) const {
    auto out = opts.output_prefix;
    out.replace_extension(".h");
    std::ofstream h(out, std::ios::binary);
    if (!h) throw std::runtime_error("Cannot open output file: " + out.string());

    auto tasks = filter(model, "TASK");
    auto alarms = filter(model, "ALARM");
    auto counters = filter(model, "COUNTER");
    auto events = filter(model, "EVENT");
    auto resources = filter(model, "RESOURCE");
    auto appmodes = filter(model, "APPMODE");
    auto messages = filter(model, "MESSAGE");
    auto networkMessages = filter(model, "NETWORKMESSAGE");
    auto ipdus = filter(model, "IPDU");
    auto isrs = filter(model, "ISR");
    auto msgStats = classifyMessages(messages);
    std::vector<std::pair<std::string, std::string>> hwDrivers;
    for (const auto* ctr : counters) {
        auto type = findAttr(*ctr, "TYPE");
        auto driver = extractDriver(type, findAttr(*ctr, "DRIVER"));
        if (!driver) driver = findAttr(*ctr, "HC12_TYPE");
        if (type && toUpper(*type).find("HARDWARE") != std::string::npos && driver) {
            auto drvName = *driver;
            auto exists = std::find_if(hwDrivers.begin(), hwDrivers.end(), [&](const auto& p) { return sanitize(p.first) == sanitize(drvName); });
            if (exists == hwDrivers.end()) {
                hwDrivers.emplace_back(drvName, ctr->name);
            }
        }
    }
    bool hasNonPreemptTask = std::any_of(tasks.begin(), tasks.end(), [](const ObjectInstance* t) {
        return toUpper(findAttr(*t, "SCHEDULE").value_or("FULL")) == "NON";
    });
    std::vector<const ObjectInstance*> internalResources;
    bool hasSchedulerResource = hasNonPreemptTask;
    for (const auto* r : resources) {
        auto prop = toUpper(findAttr(*r, "RESOURCEPROPERTY").value_or(""));
        if (prop.find("INTERNAL") != std::string::npos) internalResources.push_back(r);
        if (prop.find("SCHEDULER") != std::string::npos || toUpper(r->name).find("SCHEDULER") != std::string::npos) hasSchedulerResource = true;
    }
    size_t internalResourceCount = internalResources.size() + (hasSchedulerResource ? 1 : 0);
    bool hasInternalResourceFeature = internalResourceCount > 0;
    auto conformance = computeConformance(model, tasks);
    bool anyAutostartAlarms = std::any_of(alarms.begin(), alarms.end(), [](const ObjectInstance* a) {
        return toBool(findAttr(*a, "AUTOSTART"), false);
    });

    h << "/* Auto-generated by kosgen_cpp */\n";
    h << "#ifndef OS_CFG_H\n#define OS_CFG_H\n\n";
    h << "#include <stdint.h>\n";
    h << "#include \"Std_Types.h\"\n\n";
    h << "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n";
    h << "#define KOS_MEMORY_MAPPING             STD_OFF\n";
    h << "#define OS_AUTOSAR\n";
    h << "#define OS_FEATURE_ORTI_DEBUG          STD_ON\n";
    h << "#define OS_STANDARD_STATUS\n";
    h << "#define OS_USE_STARTUPHOOK             STD_ON\n";
    h << "#define OS_USE_RESSCHEDULER\n";
    h << "#define OS_USE_O1SCHEDULER             STD_ON\n";
    h << "#define OS_SCHED_POLICY_MIX\n";
    if (conformance.cc == "BCC1" || conformance.cc == "BCC2" || conformance.cc == "ECC1" || conformance.cc == "ECC2") {
        h << "#define OS_" << conformance.cc << "\n";
    } else {
        h << "#define OS_BCC1\n";
    }
    h << "#define OS_USE_ISRS                    STD_ON\n";
    h << "#define OS_USE_ALARMS                  " << (alarms.empty() ? "STD_OFF" : "STD_ON") << "\n";
    h << "#define OS_FEATURE_RESOURCES           " << (resources.empty() ? "STD_OFF" : "STD_ON") << "\n";
    h << "#define OS_FEATURE_INTERNAL_RESOURCES  " << (hasInternalResourceFeature ? "STD_ON" : "STD_OFF") << "\n";
    h << "#define OS_FEATURE_COM                 " << (messages.empty() ? "STD_OFF" : "STD_ON") << "\n";
    h << "#define OS_FEATURE_AUTOSTART_TASKS\n";
    if (anyAutostartAlarms) {
        h << "#define OS_FEATURE_AUTOSTART_ALARMS    STD_ON\n";
    }
    h << "#define OS_NUMBER_OF_INT_RESOURCES     " << internalResourceCount << "\n";
    h << "#define OS_RES_SCHEDULER               " << (hasSchedulerResource ? 1 : 0) << "\n";
    h << "#define OS_NUMBER_OF_EXTERNAL_TIMERS   " << hwDrivers.size() << "\n";
    h << "#define OS_NUMBER_OF_INTERNAL_TIMERS   0\n\n";
    const auto total_tasks = tasks.size() + 1;  // include idle task
    h << "#define OS_CFG_OIL_VERSION \"" << model.oil_version << "\"\n";
    h << "#define OS_NUMBER_OF_TASKS " << total_tasks << "\n";
    h << "#define OS_NUMBER_OF_ALARMS " << alarms.size() << "\n";
    h << "#define OS_NUMBER_OF_COUNTERS " << counters.size() << "\n";
    h << "#define OS_NUMBER_OF_EVENTS " << events.size() << "\n";
    h << "#define OS_NUMBER_OF_RESOURCES " << resources.size() << "\n";
    h << "#define OS_NUMBER_OF_APPMODES " << appmodes.size() << "\n";
    h << "#define OS_NUMBER_OF_ISRS " << (isrs.size() + hwDrivers.size()) << "\n";
    h << "#define OS_NUMBER_OF_NETWORKMESSAGES " << networkMessages.size() << "\n";
    h << "#define OS_NUMBER_OF_IPDUS " << ipdus.size() << "\n";
    h << "#define OS_NUMBER_OF_MESSAGES " << messages.size() << "\n";
    h << "#define OS_NUMBER_OF_INTERNAL_MESSAGES " << msgStats.internal_count << "\n";
    h << "#define OS_NUMBER_OF_EXTERNAL_MESSAGES " << msgStats.external_count << "\n";
    h << "#define COM_NUMBER_OF_INTERNAL_MESSAGES ((uint8)" << msgStats.internal_count << ")\n";
    h << "#define COM_NUMBER_OF_EXTERNAL_MESSAGES ((uint8)" << msgStats.external_count << ")\n";
    h << "#define COM_NUMBER_OF_MESSAGES (COM_NUMBER_OF_INTERNAL_MESSAGES + COM_NUMBER_OF_EXTERNAL_MESSAGES)\n";
    auto mlq_prios = tasks.size();
    h << "#define OS_DISTINCT_PRIORITIES      ((uint8)" << mlq_prios << ")\n";
    h << "#define OS_MLQ_NUMBER_OF_PRIORITIES OS_DISTINCT_PRIORITIES\n";
    h << "#define OS_MLQ_QUEUE_SIZE           ((uint8)" << mlq_prios << ")\n\n";

    if (alarms.size() > 16) {
        h << "typedef uint32 Os_AlarmStateType;\n";
    } else if (alarms.size() > 8) {
        h << "typedef uint16 Os_AlarmStateType;\n";
    } else {
        h << "typedef uint8 Os_AlarmStateType;\n";
    }
    h << "\n";

    h << "#include \"Os_Types.h\"\n";
    h << "#include \"Os_Defs.h\"\n";
    h << "#include \"Os_Port.h\"\n";
    h << "#include \"Com.h\"\n\n";

    h << "typedef enum {\n";
    h << "    TASK_ID_OsExec_IdleTask = 0,\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        h << "    TASK_ID_" << sanitize(tasks[i]->name) << " = " << (i + 1) << ",\n";
    }
    h << "    TASK_ID_COUNT = " << total_tasks << "\n";
    h << "} Os_TaskId;\n";
    h << "#define OsExec_IdleTask ((uint8_t)TASK_ID_OsExec_IdleTask)\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        h << "#define " << sanitize(tasks[i]->name) << " ((uint8_t)TASK_ID_" << sanitize(tasks[i]->name) << ")\n";
    }
    h << "extern const char* const Os_TaskNames[OS_NUMBER_OF_TASKS];\n";
    h << "extern const uint32_t Os_TaskPriorities[OS_NUMBER_OF_TASKS];\n\n";
    h << "extern const uint8_t Os_TaskAutostart[OS_NUMBER_OF_TASKS];\n";
    h << "extern const uint32_t Os_TaskActivation[OS_NUMBER_OF_TASKS];\n";
    h << "extern const uint32_t Os_TaskStackSize[OS_NUMBER_OF_TASKS];\n";
    h << "extern const char* const Os_TaskSchedule[OS_NUMBER_OF_TASKS];\n";
    h << "extern const char* const Os_TaskType[OS_NUMBER_OF_TASKS];\n";
    h << "extern const char* const Os_TaskAutostartAppmodes[OS_NUMBER_OF_TASKS];\n";
    if (!messages.empty()) {
        h << "extern const char* const Os_MessageNames[OS_NUMBER_OF_MESSAGES];\n";
        h << "typedef struct {\n";
        h << "    Com_FilterAlgorithm filter;\n";
        h << "    uint64_t mask;\n";
        h << "    uint64_t x;\n";
        h << "    uint64_t min;\n";
        h << "    uint64_t max;\n";
        h << "    uint64_t period;\n";
        h << "    uint64_t offset;\n";
        h << "    uint64_t initial_value;\n";
        h << "    uint32_t queue_size;\n";
        h << "    uint16_t link_message;\n";
        h << "    uint8_t link_enabled;\n";
        h << "    uint8_t is_queued;\n";
        h << "} Os_MessageFilterConfig;\n";
        h << "extern const Os_MessageFilterConfig Os_MessageFilters[COM_NUMBER_OF_MESSAGES];\n";
        h << "extern const int16_t Os_MessageNetworkId[COM_NUMBER_OF_MESSAGES];\n";
    }
    if (!networkMessages.empty()) {
        h << "extern const char* const Os_NetworkMessageNames[OS_NUMBER_OF_NETWORKMESSAGES];\n";
        h << "typedef struct {\n";
        h << "    int16_t ipdu_id;\n";
        h << "    const char* property;\n";
        h << "    uint32_t size_bits;\n";
        h << "    uint32_t max_size_bits;\n";
        h << "    uint32_t bit_position;\n";
        h << "    const char* bit_order;\n";
        h << "    const char* interpretation;\n";
        h << "    const char* direction;\n";
        h << "    const char* transfer;\n";
        h << "    uint64_t initial_value;\n";
        h << "} Os_NetworkMessageConfig;\n";
        h << "extern const Os_NetworkMessageConfig Os_NetworkMessages[OS_NUMBER_OF_NETWORKMESSAGES];\n";
    }
    if (!ipdus.empty()) {
        h << "extern const char* const Os_IpduNames[OS_NUMBER_OF_IPDUS];\n";
        h << "typedef struct {\n";
        h << "    const char* property;\n";
        h << "    const char* transmission_mode;\n";
        h << "    uint32_t size_bits;\n";
        h << "    uint64_t period;\n";
        h << "    uint64_t offset;\n";
        h << "    uint64_t min_delay;\n";
        h << "    uint64_t timeout;\n";
        h << "    uint64_t first_timeout;\n";
        h << "    const char* callout;\n";
        h << "    const char* layer;\n";
        h << "} Os_IpduConfig;\n";
        h << "extern const Os_IpduConfig Os_Ipdus[OS_NUMBER_OF_IPDUS];\n";
    }
    h << "typedef struct {\n";
    h << "    uint8_t autostart;\n";
    h << "    uint32_t activation;\n";
    h << "    uint32_t priority;\n";
    h << "    uint32_t stacksize;\n";
    h << "    const char* schedule;\n";
    h << "    const char* type;\n";
    h << "    const char* autostart_appmodes;\n";
    h << "} Os_TaskConfig;\n";
    h << "extern const Os_TaskConfig Os_Tasks[OS_NUMBER_OF_TASKS];\n\n";

    if (!alarms.empty()) {
        h << "typedef enum {\n";
        for (size_t i = 0; i < alarms.size(); ++i) {
            h << "    ALARM_ID_" << sanitize(alarms[i]->name) << " = " << i << ",\n";
        }
        h << "    ALARM_ID_COUNT = " << alarms.size() << "\n";
        h << "} Os_AlarmId;\n";
        for (size_t i = 0; i < alarms.size(); ++i) {
            h << "#define " << sanitize(alarms[i]->name) << " ((uint8_t)ALARM_ID_" << sanitize(alarms[i]->name) << ")\n";
        }
        h << "extern const char* const Os_AlarmNames[OS_NUMBER_OF_ALARMS];\n";
        h << "typedef struct {\n";
        h << "    uint8_t counter_id;\n";
        h << "    uint8_t action_task_id;\n";
        h << "    const char* action_type;\n";
        h << "    uint8_t autostart;\n";
        h << "    const char* autostart_appmodes;\n";
        h << "} Os_AlarmConfig;\n";
        h << "extern const Os_AlarmConfig Os_Alarms[OS_NUMBER_OF_ALARMS];\n";
    }
    if (!counters.empty()) {
        h << "typedef enum {\n";
        for (size_t i = 0; i < counters.size(); ++i) {
            h << "    COUNTER_ID_" << sanitize(counters[i]->name) << " = " << i << ",\n";
        }
        h << "    COUNTER_ID_COUNT = " << counters.size() << "\n";
        h << "} Os_CounterId;\n";
        for (size_t i = 0; i < counters.size(); ++i) {
            h << "#define " << sanitize(counters[i]->name) << " ((uint8_t)COUNTER_ID_" << sanitize(counters[i]->name) << ")\n";
        }
        h << "extern const char* const Os_CounterNames[OS_NUMBER_OF_COUNTERS];\n";
        h << "typedef struct {\n";
        h << "    uint32_t max_allowed_value;\n";
        h << "    uint32_t ticks_per_base;\n";
        h << "    uint32_t min_cycle;\n";
        h << "} Os_CounterConfig;\n";
        h << "extern const Os_CounterConfig Os_Counters[OS_NUMBER_OF_COUNTERS];\n";
        for (size_t i = 0; i < counters.size(); ++i) {
            auto type = findAttr(*counters[i], "TYPE");
            auto driver = extractDriver(type, findAttr(*counters[i], "DRIVER"));
            if (!driver) driver = findAttr(*counters[i], "HC12_TYPE");
            if (type && toUpper(*type).find("HARDWARE") != std::string::npos && driver) {
                std::string drvUp = toUpper(sanitize(*driver));
                h << "#define OS_FEATURE_INSTALL_" << drvUp << "_HANDLER\n";
                h << "#define OS_" << drvUp << "_TIMER_DRIVER_IMPL()           \\\n";
                h << "    _BEGIN_BLOCK                             \\\n";
                h << "        (void) IncrementCounter(" << sanitize(counters[i]->name) << "); \\\n";
                h << "    _END_BLOCK\n";
                auto drvName = *driver;
                auto exists = std::find_if(hwDrivers.begin(), hwDrivers.end(), [&](const auto& p) { return sanitize(p.first) == sanitize(drvName); });
                if (exists == hwDrivers.end()) {
                    hwDrivers.emplace_back(drvName, counters[i]->name);
                }
            }
        }
    }
    if (!events.empty()) {
        h << "extern const char* const Os_EventNames[OS_NUMBER_OF_EVENTS];\n";
        for (size_t i = 0; i < events.size(); ++i) {
            h << "#define " << sanitize(events[i]->name) << " ((EventMaskType)(1u << " << i << "))\n";
        }
        h << "extern const EventMaskType Os_EventMask[OS_NUMBER_OF_EVENTS];\n";
    }
    if (!resources.empty()) {
        h << "typedef enum {\n";
        for (size_t i = 0; i < resources.size(); ++i) {
            h << "    RESOURCE_ID_" << sanitize(resources[i]->name) << " = " << i << ",\n";
        }
        h << "    RESOURCE_ID_COUNT = " << resources.size() << "\n";
        h << "} Os_ResourceId;\n";
        for (size_t i = 0; i < resources.size(); ++i) {
            h << "#define " << sanitize(resources[i]->name) << " ((uint8_t)RESOURCE_ID_" << sanitize(resources[i]->name) << ")\n";
        }
        h << "extern const char* const Os_ResourceNames[OS_NUMBER_OF_RESOURCES];\n";
        h << "extern const char* const Os_ResourceProperty[OS_NUMBER_OF_RESOURCES];\n";
        h << "extern const Os_ResourceConfigurationType OS_ResourceConf[OS_NUMBER_OF_RESOURCES];\n";
        h << "extern Os_ResourceType Os_Resources[OS_NUMBER_OF_RESOURCES];\n";
    }
    if (hasInternalResourceFeature) {
        h << "extern const Os_ResourceConfigurationType OS_IntResourceConf[OS_NUMBER_OF_INT_RESOURCES];\n";
    }
    if (!appmodes.empty()) {
        h << "typedef enum {\n";
        for (size_t i = 0; i < appmodes.size(); ++i) {
            h << "    APPMODE_ID_" << sanitize(appmodes[i]->name) << " = " << (i + 1) << ",\n";
        }
        h << "    APPMODE_ID_COUNT = " << appmodes.size() << "\n";
        h << "} Os_AppModeId;\n";
        for (size_t i = 0; i < appmodes.size(); ++i) {
            h << "#define " << sanitize(appmodes[i]->name) << " ((uint8_t)APPMODE_ID_" << sanitize(appmodes[i]->name) << ")\n";
        }
        h << "extern const char* const Os_AppModeNames[OS_NUMBER_OF_APPMODES];\n";
        h << "#define OS_FEATURE_REAL_DEFAULT_APPMODE ((AppModeType)" << (appmodes.empty() ? 0 : 1) << ")\n";
    }
    if (!messages.empty()) {
        h << "typedef enum {\n";
        for (size_t i = 0; i < messages.size(); ++i) {
            h << "    MESSAGE_ID_" << sanitize(messages[i]->name) << " = " << i << ",\n";
        }
        h << "    MESSAGE_ID_COUNT = " << messages.size() << "\n";
        h << "} Os_MessageId;\n";
        for (size_t i = 0; i < messages.size(); ++i) {
            h << "#define " << sanitize(messages[i]->name) << " ((uint8_t)MESSAGE_ID_" << sanitize(messages[i]->name) << ")\n";
        }
    }
    if (!isrs.empty() || !hwDrivers.empty()) {
        h << "typedef enum {\n";
        for (size_t i = 0; i < isrs.size(); ++i) {
            h << "    ISR_ID_" << sanitize(isrs[i]->name) << " = " << i << ",\n";
        }
        for (size_t i = 0; i < hwDrivers.size(); ++i) {
            h << "    ISR_ID_" << sanitize(hwDrivers[i].first) << "Timer = " << (isrs.size() + i) << ",\n";
        }
        h << "    ISR_ID_COUNT = " << (isrs.size() + hwDrivers.size()) << "\n";
        h << "} Os_IsrId;\n";
        for (size_t i = 0; i < isrs.size(); ++i) {
            h << "#define " << sanitize(isrs[i]->name) << " ((uint8_t)ISR_ID_" << sanitize(isrs[i]->name) << ")\n";
        }
        for (size_t i = 0; i < hwDrivers.size(); ++i) {
            h << "#define " << sanitize(hwDrivers[i].first) << "Timer ((uint8_t)ISR_ID_" << sanitize(hwDrivers[i].first) << "Timer)\n";
        }
        if (!hwDrivers.empty()) {
            h << "typedef enum {\n";
            h << "    ISRID_NONE,\n";
            for (size_t i = 0; i < hwDrivers.size(); ++i) {
                h << "    ISRID_" << sanitize(hwDrivers[i].first) << "Timer";
                if (i + 1 < hwDrivers.size()) h << ",";
                h << "\n";
            }
            h << "} ISRType_Generated;\n";
        }
        h << "extern const char* const Os_IsrNames[OS_NUMBER_OF_ISRS];\n";
    }
    h << "\n";

    h << "/* Forward declarations for Tasks and ISRs */\n";
    h << "DeclareTask(OsExec_IdleTask);\n";
    for (const auto* t : tasks) {
        h << "DeclareTask(" << sanitize(t->name) << ");\n";
    }
    for (const auto* isr : isrs) {
        h << "ISR2(" << sanitize(isr->name) << ");\n";
    }
    for (const auto& drv : hwDrivers) {
        h << "ISR2(" << sanitize(drv.first) << "Timer);\n";
    }
    h << "\n";
    h << "extern const char* const Os_OilVersion;\n\n";

    h << "#define OS_DEFAULT_STACK_SIZE    32\n";
    h << "#define IDLE_TASK_STACK_SIZE ((uint8)" << 32 << ")\n";
    h << "#define ISR_STACK_SIZE ((uint8)" << 32 << ")\n";
    for (const auto* t : tasks) {
        auto ss = toNumber(findAttr(*t, "STACKSIZE"), 0);
        if (ss == 0) ss = 32;
        h << "#define " << stackSizeMacro(t->name) << " ((uint8)" << ss << ")\n";
    }
    h << "extern uint8_t IdleTask_Stack[IDLE_TASK_STACK_SIZE];\n";
    h << "extern uint8_t ISR_Stack[ISR_STACK_SIZE];\n";
    for (const auto* t : tasks) {
        auto ss = toNumber(findAttr(*t, "STACKSIZE"), 0);
        if (ss == 0) ss = 32;
        h << "extern uint8_t " << sanitize(t->name) << "_Stack[" << stackSizeMacro(t->name) << "];\n";
    }
    h << "extern const Os_TaskConfigurationType OS_TaskConf[OS_NUMBER_OF_TASKS];\n";
    h << "extern Os_TCBType OS_TCB[OS_NUMBER_OF_TASKS];\n";
    h << "extern volatile TickType Os_CounterValues[OS_NUMBER_OF_COUNTERS];\n";
    h << "extern const CounterConfigurationType Os_CounterDefs[OS_NUMBER_OF_COUNTERS];\n";
    h << "extern const AlarmConfigurationType OS_AlarmConf[OS_NUMBER_OF_ALARMS];\n";
    h << "extern volatile OSAlarm OS_AlarmValue[OS_NUMBER_OF_ALARMS];\n";
    h << "extern const Com_MessageObjectType Com_MessageObjects[COM_NUMBER_OF_MESSAGES];\n";

    for (size_t i = 0; i < counters.size(); ++i) {
        auto maxv = toNumber(findAttr(*counters[i], "MAXALLOWEDVALUE"), 0);
        auto ticks = toNumber(findAttr(*counters[i], "TICKSPERBASE"), 0);
        auto minc = toNumber(findAttr(*counters[i], "MINCYCLE"), 0);
        std::string upper = toUpper(counters[i]->name);
        h << "#define OSMAXALLOWEDVALUE_" << upper << "    (" << maxv << ")\n";
        h << "#define OSTICKSPERBASE_" << upper << "       (" << ticks << ")\n";
        h << "#define OSMINCYCLE_" << upper << "           (" << minc << ")\n";
        h << "#define OS_TICKS2SEC_" << upper << "(ticks)  ((ticks)/1000)\n";
        h << "#define OS_TICKS2MS_" << upper << "(ticks)   (ticks)\n";
        h << "#define OS_TICKS2US_" << upper << "(ticks)   ((ticks)*1000)\n";
        h << "#define OS_TICKS2NS_" << upper << "(ticks)   ((ticks)*1000000)\n";
    }
    h << "#define OS_TC2_DRIVER_TBC_THRESHOLD   (0)\n\n";
    h << "extern const OsMLQ_QueueConfigurationType MLQ_QueueDef[OS_MLQ_NUMBER_OF_PRIORITIES];\n\n";

    h << "#ifdef __cplusplus\n}\n#endif\n\n";
    h << "#endif /* OS_CFG_H */\n";
}

void Generator::writeOsCfgC(const OilModel& model, const Options& opts) const {
    auto out = opts.output_prefix;
    out.replace_extension(".c");
    std::ofstream c(out, std::ios::binary);
    if (!c) throw std::runtime_error("Cannot open output file: " + out.string());

    auto tasks = filter(model, "TASK");
    auto alarms = filter(model, "ALARM");
    auto counters = filter(model, "COUNTER");
    auto events = filter(model, "EVENT");
    auto resources = filter(model, "RESOURCE");
    auto appmodes = filter(model, "APPMODE");
    auto messages = filter(model, "MESSAGE");
    auto networkMessages = filter(model, "NETWORKMESSAGE");
    auto ipdus = filter(model, "IPDU");
    auto isrs = filter(model, "ISR");
    bool anyAutostartAlarms = std::any_of(alarms.begin(), alarms.end(), [](const ObjectInstance* a) {
        return toBool(findAttr(*a, "AUTOSTART"), false);
    });
    auto conformance = computeConformance(model, tasks);
    auto taskIndex = indexByName(tasks);
    auto counterIndex = indexByName(counters);
    std::vector<std::string> hwDriversC;
    for (const auto* ctr : counters) {
        auto type = findAttr(*ctr, "TYPE");
        auto driver = extractDriver(type, findAttr(*ctr, "DRIVER"));
        if (!driver) driver = findAttr(*ctr, "HC12_TYPE");
        if (type && toUpper(*type).find("HARDWARE") != std::string::npos && driver) {
            auto sanitized = sanitize(*driver);
            if (std::find(hwDriversC.begin(), hwDriversC.end(), sanitized) == hwDriversC.end()) {
                hwDriversC.push_back(sanitized);
            }
        }
    }
    auto messageIndex = indexByName(messages);
    auto networkMessageIndex = indexByName(networkMessages);
    auto ipduIndex = indexByName(ipdus);
    auto resourceIndex = indexByName(resources);
    auto eventIndex = indexByName(events);
    bool hasNonPreemptTask = std::any_of(tasks.begin(), tasks.end(), [](const ObjectInstance* t) {
        return toUpper(findAttr(*t, "SCHEDULE").value_or("FULL")) == "NON";
    });
    std::vector<const ObjectInstance*> internalResources;
    bool hasSchedulerResource = hasNonPreemptTask;
    for (const auto* r : resources) {
        auto prop = toUpper(findAttr(*r, "RESOURCEPROPERTY").value_or(""));
        if (prop.find("INTERNAL") != std::string::npos) internalResources.push_back(r);
        if (prop.find("SCHEDULER") != std::string::npos || toUpper(r->name).find("SCHEDULER") != std::string::npos) hasSchedulerResource = true;
    }
    size_t internalResourceCount = internalResources.size() + (hasSchedulerResource ? 1 : 0);
    bool hasInternalResourceFeature = internalResourceCount > 0;
    std::vector<uint32_t> resourceCeil(resources.size(), 0);
    for (const auto* t : tasks) {
        auto resAttr = findAttr(*t, "RESOURCE");
        if (!resAttr) continue;
        auto prio = toNumber(findAttr(*t, "PRIORITY"), 0);
        std::regex token(R"([A-Za-z_]\w*)");
        for (std::sregex_iterator it(resAttr->begin(), resAttr->end(), token), end; it != end; ++it) {
            auto name = (*it)[0].str();
            auto idxIt = resourceIndex.find(name);
            if (idxIt == resourceIndex.end()) {
                auto upperName = toUpper(name);
                for (const auto& kv : resourceIndex) {
                    if (toUpper(kv.first) == upperName) {
                        idxIt = resourceIndex.find(kv.first);
                        break;
                    }
                }
            }
            if (idxIt != resourceIndex.end()) {
                auto idx = idxIt->second;
                if (prio > resourceCeil[idx]) resourceCeil[idx] = prio;
            }
        }
    }
    auto resolveInternalResourceIndex = [&](const std::string& name) -> std::optional<size_t> {
        auto upper = toUpper(name);
        for (size_t i = 0; i < internalResources.size(); ++i) {
            if (toUpper(internalResources[i]->name) == upper) {
                return hasSchedulerResource ? i + 1 : i;
            }
        }
        return std::nullopt;
    };
    auto findTaskInternalResource = [&](const ObjectInstance* task) -> std::optional<size_t> {
        auto resAttr = findAttr(*task, "RESOURCE");
        if (!resAttr) return std::nullopt;
        std::regex token(R"([A-Za-z_]\w*)");
        for (std::sregex_iterator it(resAttr->begin(), resAttr->end(), token), end; it != end; ++it) {
            auto name = (*it)[0].str();
            auto idx = resolveInternalResourceIndex(name);
            if (idx) return idx;
        }
        return std::nullopt;
    };
    constexpr size_t defaultStack = 32;
    auto resolveTaskId = [&](const std::optional<std::string>& name) -> uint8_t {
        if (!name) return 0;
        auto it = taskIndex.find(*name);
        if (it != taskIndex.end()) return static_cast<uint8_t>(it->second + 1);
        auto upper = toUpper(*name);
        for (const auto& kv : taskIndex) {
            if (toUpper(kv.first) == upper) return static_cast<uint8_t>(kv.second + 1);
        }
        return 0;
    };
    std::vector<std::pair<std::string, std::string>> hwDrivers;  // pair<driverSymbol, counterName>

    auto messageType = [&](const ObjectInstance* m) {
        auto typeAttr = findAttr(*m, "CDATATYPE");
        std::string typeStr = typeAttr.value_or("");
        if (typeStr.empty()) {
            auto sending = findAttr(*m, "SENDINGMESSAGE");
            if (sending) {
                auto it = messageIndex.find(*sending);
                if (it != messageIndex.end()) {
                    auto ref = messages[it->second];
                    auto refType = findAttr(*ref, "CDATATYPE");
                    typeStr = refType.value_or("");
                }
            }
        }
        if (!typeStr.empty() && (typeStr.front() == '"' || typeStr.front() == '\'')) {
            if (typeStr.size() > 1) typeStr = typeStr.substr(1, typeStr.size() - 2);
        }
        typeStr = toUpper(typeStr);
        return typeStr;
    };

    auto messageSize = [&](const ObjectInstance* m) {
        auto typeStr = messageType(m);
        if (typeStr.find("LONG") != std::string::npos || typeStr.find("UINT32") != std::string::npos ||
            typeStr.find("INT32") != std::string::npos)
            return 4;
        if (typeStr.find("SHORT") != std::string::npos || typeStr.find("UINT16") != std::string::npos ||
            typeStr.find("INT16") != std::string::npos)
            return 2;
        if (typeStr.find("CHAR") != std::string::npos || typeStr.find("UINT8") != std::string::npos ||
            typeStr.find("INT8") != std::string::npos)
            return 1;
        if (typeStr.find("FLOAT") != std::string::npos)
            return 4;
        return 4;
    };

    auto resolveEventMask = [&](const std::optional<std::string>& name) -> uint32_t {
        if (!name) return 0;
        auto it = eventIndex.find(*name);
        if (it != eventIndex.end()) return 1u << it->second;
        auto upper = toUpper(*name);
        for (const auto& kv : eventIndex) {
            if (toUpper(kv.first) == upper) return 1u << kv.second;
        }
        return 0;
    };

    c << "/* Auto-generated by kosgen_cpp */\n";
    c << "#include \"Os_Cfg.h\"\n";
    c << "#include \"Os_Defs.h\"\n";
    c << "#include \"Os_Port.h\"\n";
    c << "#include \"Os_Vars.h\"\n";
    c << "#include \"Os_Exec.h\"\n";
    if (!messages.empty()) {
        c << "#include \"Com.h\"\n";
    }
    c << "\nconst char* const Os_OilVersion = \"" << model.oil_version << "\";\n\n";

    c << "const char* const Os_TaskNames[OS_NUMBER_OF_TASKS] = {\"OsExec_IdleTask\"";
    for (const auto* t : tasks) c << ", \"" << t->name << "\"";
    c << "};\n";

    c << "const uint32_t Os_TaskPriorities[OS_NUMBER_OF_TASKS] = {0";
    for (const auto* t : tasks) {
        auto prio = toNumber(findAttr(*t, "PRIORITY"), 1);
        c << ", " << prio;
    }
    c << "};\n";

    c << "const uint8_t Os_TaskAutostart[OS_NUMBER_OF_TASKS] = {1";
    for (const auto* t : tasks) {
        auto autostart = toBool(findAttr(*t, "AUTOSTART"), false) ? 1 : 0;
        c << ", " << static_cast<unsigned>(autostart);
    }
    c << "};\n";

    c << "const uint32_t Os_TaskActivation[OS_NUMBER_OF_TASKS] = {1";
    for (const auto* t : tasks) {
        auto act = toNumber(findAttr(*t, "ACTIVATION"), 1);
        c << ", " << act;
    }
    c << "};\n";

    c << "const uint32_t Os_TaskStackSize[OS_NUMBER_OF_TASKS] = {" << defaultStack;
    for (const auto* t : tasks) {
        auto sz = toNumber(findAttr(*t, "STACKSIZE"), 0);
        if (sz == 0) sz = defaultStack;
        c << ", " << sz;
    }
    c << "};\n";

    c << "const char* const Os_TaskSchedule[OS_NUMBER_OF_TASKS] = {\"FULL\"";
    for (const auto* t : tasks) {
        auto sched = findAttr(*t, "SCHEDULE").value_or("NON");
        c << ", \"" << sched << "\"";
    }
    c << "};\n";

    c << "const char* const Os_TaskType[OS_NUMBER_OF_TASKS] = {\"BASIC\"";
    for (const auto* t : tasks) {
        auto type = findAttr(*t, "TYPE").value_or("BASIC");
        c << ", \"" << type << "\"";
    }
    c << "};\n";

    c << "const char* const Os_TaskAutostartAppmodes[OS_NUMBER_OF_TASKS] = {\"OS_AUTOSTART_ALWAYS\"";
    for (const auto* t : tasks) {
        auto joined = joinAppModes(findAttr(*t, "AUTOSTART"));
        if (joined.empty()) joined = joinAppModes(findAttr(*t, "APPMODE"));
        c << ", \"" << joined << "\"";
    }
    c << "};\n\n";

    if (!isrs.empty() || !hwDriversC.empty()) {
        c << "const char* const Os_IsrNames[OS_NUMBER_OF_ISRS] = {";
        bool first = true;
        for (const auto* isr : isrs) {
            if (!first) c << ", ";
            c << "\"" << isr->name << "\"";
            first = false;
        }
        for (const auto& drv : hwDriversC) {
            if (!first) c << ", ";
            c << "\"" << drv << "Timer\"";
            first = false;
        }
        c << "};\n\n";
    }

    c << "uint8_t IdleTask_Stack[IDLE_TASK_STACK_SIZE];\n";
    c << "uint8_t ISR_Stack[ISR_STACK_SIZE];\n";
    for (const auto* t : tasks) {
        auto ss = toNumber(findAttr(*t, "STACKSIZE"), 0);
        if (ss == 0) ss = defaultStack;
        c << "uint8_t " << sanitize(t->name) << "_Stack[" << stackSizeMacro(t->name) << "];\n";
    }
    c << "\n";

    c << "const Os_TaskConfigurationType OS_TaskConf[OS_NUMBER_OF_TASKS] = {\n";
    c << "    { GetTaskName(OsExec_IdleTask), IdleTask_Stack, IDLE_TASK_STACK_SIZE, ((PriorityType)0), OS_TASK_ATTR_FULLPREEMPT";
    if (conformance.hasMaxActivations) c << ", 1";
    c << ", OS_AUTOSTART_ALWAYS";
    if (hasInternalResourceFeature) c << ", INTERNAL_RES_NONE";
    c << " },\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        auto ss = toNumber(findAttr(*tasks[i], "STACKSIZE"), 0);
        if (ss == 0) ss = defaultStack;
        auto prio = toNumber(findAttr(*tasks[i], "PRIORITY"), 1);
        auto sched = findAttr(*tasks[i], "SCHEDULE").value_or("NON");
        auto type = findAttr(*tasks[i], "TYPE").value_or("BASIC");
        auto activationCount = toNumber(findAttr(*tasks[i], "ACTIVATION"), 1);
        auto explicitInternalRes = findTaskInternalResource(tasks[i]);
        uint8_t flags = 0;
        std::string sched_upper = toUpper(sched);
        if (sched_upper == "FULL") flags |= 0x01;
        std::string type_upper = toUpper(type);
        if (type_upper == "EXTENDED") flags |= 0x02;
        auto autostart = toBool(findAttr(*tasks[i], "AUTOSTART"), false);
        const char* autostartMode = autostart ? "DefaultAppMode" : "OS_AUTOSTART_NEVER";
        std::string internalRes = "INTERNAL_RES_NONE";
        if (explicitInternalRes) {
            internalRes = "(ResourceType)" + std::to_string(*explicitInternalRes);
            flags |= 0x04;
        } else if (hasSchedulerResource && sched_upper == "NON") {
            internalRes = "INTERNAL_RES_SCHEDULER";
            flags |= 0x04;
        }
        c << "    { GetTaskName(" << sanitize(tasks[i]->name) << "), " << sanitize(tasks[i]->name) << "_Stack, " << stackSizeMacro(tasks[i]->name)
          << ", ((PriorityType)" << prio << "), " << static_cast<unsigned>(flags);
        if (conformance.hasMaxActivations) c << ", " << activationCount;
        c << ", " << autostartMode;
        if (hasInternalResourceFeature) c << ", " << internalRes;
        c << " }";
        if (i + 1 < tasks.size()) c << ",";
        c << "\n";
    }
    c << "};\n\n";

    c << "Os_TCBType OS_TCB[OS_NUMBER_OF_TASKS];\n\n";

    emitStringArray(c, "Os_AlarmNames", alarms);
    c << '\n';

    c << "volatile OSAlarm OS_AlarmValue[OS_NUMBER_OF_ALARMS];\n";
    c << "volatile TickType Os_CounterValues[OS_NUMBER_OF_COUNTERS];\n\n";

    for (size_t ci = 0; ci < counters.size(); ++ci) {
        size_t alarmCount = 0;
        for (const auto* a : alarms) {
            auto cn = findAttr(*a, "COUNTER");
            if (cn && *cn == counters[ci]->name) ++alarmCount;
        }
        c << "const AlarmType OS_Alarms_" << sanitize(counters[ci]->name) << "[" << (alarmCount == 0 ? 1 : alarmCount) << "] = {";
        bool first = true;
        for (size_t ai = 0; ai < alarms.size(); ++ai) {
            auto counter_name = findAttr(*alarms[ai], "COUNTER");
            if (counter_name && *counter_name == counters[ci]->name) {
                if (!first) c << ", ";
                c << sanitize(alarms[ai]->name);
                first = false;
            }
        }
        if (first) c << "0";
        c << "};\n";
    }
    c << "\n";

    c << "const CounterConfigurationType Os_CounterDefs[OS_NUMBER_OF_COUNTERS] = {\n";
    for (size_t i = 0; i < counters.size(); ++i) {
        std::string upper = toUpper(counters[i]->name);
        size_t alarmCount = 0;
        for (const auto* a : alarms) {
            auto cn = findAttr(*a, "COUNTER");
            if (cn && *cn == counters[i]->name) ++alarmCount;
        }
        if (alarmCount == 0) alarmCount = 1;
        c << "    { &Os_CounterValues[" << i << "], { OSMAXALLOWEDVALUE_" << upper << ", OSTICKSPERBASE_" << upper << ", OSMINCYCLE_" << upper << " }, " << alarmCount
          << ", OS_Alarms_" << sanitize(counters[i]->name) << " }";
        if (i + 1 < counters.size()) c << ",";
        c << "\n";
    }
    c << "};\n\n";

    c << "const AlarmConfigurationType OS_AlarmConf[OS_NUMBER_OF_ALARMS] = {\n";
    for (size_t i = 0; i < alarms.size(); ++i) {
        auto action_raw = findAttr(*alarms[i], "ACTION");
        auto action_type_raw = parseActionType(action_raw);
        std::string action_type = "ALM_ACTIVATETASK";
        auto upper_action = toUpper(action_type_raw);
        if (upper_action == "SETEVENT") action_type = "ALM_SETEVENT";
        else if (upper_action == "ACTIVATETASK") action_type = "ALM_ACTIVATETASK";
        else if (upper_action == "ALARMCALLBACK" || upper_action == "CALLBACK") action_type = "ALM_CALLBACK";
        auto action_task_name = parseFirstToken(action_raw, "TASK");
        auto counter_name = findAttr(*alarms[i], "COUNTER");
        uint8_t counter_id = 0xFF;
        if (counter_name) {
            auto it = counterIndex.find(*counter_name);
            if (it != counterIndex.end()) counter_id = static_cast<uint8_t>(it->second);
        }
        std::string actionTaskInit = "0xFFFF";
        if (action_task_name) {
            auto it = taskIndex.find(*action_task_name);
            if (it != taskIndex.end()) actionTaskInit = sanitize(*action_task_name);
        }
        bool autostart = toBool(findAttr(*alarms[i], "AUTOSTART"), false);
        auto autostartModeToken = parseFirstToken(findAttr(*alarms[i], "AUTOSTART"), "APPMODE");
        std::string autostartMode = autostart ? (autostartModeToken ? sanitize(*autostartModeToken) : std::string("DefaultAppMode")) : std::string("OS_AUTOSTART_NEVER");
        auto alarmTime = parseNumberToken(findAttr(*alarms[i], "AUTOSTART"), "ALARMTIME").value_or(0);
        auto cycleTime = parseNumberToken(findAttr(*alarms[i], "AUTOSTART"), "CYCLETIME").value_or(0);
        c << "    { (CounterType)" << static_cast<unsigned>(counter_id) << ", " << action_type << ", { .TaskID = (TaskType)"
          << actionTaskInit << " }";
        if (anyAutostartAlarms) {
            c << ", { " << autostartMode << ", " << alarmTime << ", " << cycleTime << " }";
        }
        c << " }";
        if (i + 1 < alarms.size()) c << ",";
        c << "\n";
    }
    c << "};\n\n";

    c << "static TaskType MLQ_QueueData[OS_MLQ_QUEUE_SIZE];\n";
    c << "const OsMLQ_QueueConfigurationType MLQ_QueueDef[OS_MLQ_NUMBER_OF_PRIORITIES] = {\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        c << "    { (uint8)1, &MLQ_QueueData[" << i << "] }";
        if (i + 1 < tasks.size()) c << ",";
        c << "\n";
    }
    c << "};\n\n";
    for (const auto& drv : hwDriversC) {
        c << "IMPLEMENT_ISR2_VECTOR(" << drv << "Timer)\n";
    }
    if (!hwDriversC.empty()) c << "\n";

        if (!messages.empty()) {
            std::vector<std::vector<size_t>> receiversForMessage(messages.size());
            for (size_t i = 0; i < messages.size(); ++i) {
                auto sending = findAttr(*messages[i], "SENDINGMESSAGE");
                if (!sending) sending = parseFirstToken(findAttr(*messages[i], "MESSAGEPROPERTY"), "SENDINGMESSAGE");
                if (!sending) continue;
                auto it = messageIndex.find(*sending);
                if (it == messageIndex.end()) {
                    auto upper = toUpper(*sending);
                    for (const auto& kv : messageIndex) {
                        if (toUpper(kv.first) == upper) {
                            it = messageIndex.find(kv.first);
                            break;
                        }
                    }
                }
                if (it != messageIndex.end()) {
                    receiversForMessage[it->second].push_back(i);
                }
            }

            struct MessageGenInfo {
                std::string propUpper;
                size_t size;
                bool isReceive;
                std::string notifyType;
                std::string actionInit;
                std::string setEventForward;
                std::string setEventDef;
                std::string receiverArray;
                size_t receiverCount = 0;
                std::vector<size_t> receivers;
                uint8_t filterAlg = 0;
                uint64_t mask = 0;
                uint64_t x = 0;
                uint64_t min = 0;
                uint64_t max = 0;
                uint64_t period = 0;
                uint64_t offset = 0;
                uint64_t initialValue = 0;
                uint32_t queueSize = 0;
                uint16_t linkMessage = 0xFFFF;
                bool linkEnabled = false;
                bool isQueued = false;
                int16_t networkId = -1;
            };
            std::vector<MessageGenInfo> messageInfos;
            for (size_t idx = 0; idx < messages.size(); ++idx) {
                const auto* m = messages[idx];
                auto prop = findAttr(*m, "MESSAGEPROPERTY");
                std::string propStr = prop.value_or("");
                auto propTokenEnd = propStr.find_first_of(" {");
                auto propToken = propStr.substr(0, propTokenEnd);
                std::string propUpper = toUpper(propToken);
            size_t sz = messageSize(m);
            bool isReceive = propUpper.find("RECEIVE") != std::string::npos;
            if (isReceive) {
                auto typeStr = messageType(m);
                if (typeStr.find("LONG") != std::string::npos || typeStr.find("UINT32") != std::string::npos ||
                    typeStr.find("INT32") != std::string::npos || sz == 4) {
                    c << "static long Com_MessageData_" << sanitize(m->name) << ";\n";
                } else if (typeStr.find("SHORT") != std::string::npos || typeStr.find("UINT16") != std::string::npos ||
                           typeStr.find("INT16") != std::string::npos || sz == 2) {
                    c << "static short Com_MessageData_" << sanitize(m->name) << ";\n";
                } else {
                    c << "static uint8_t Com_MessageData_" << sanitize(m->name) << "[" << sz << "];\n";
                }
            }
            MessageGenInfo info;
            info.propUpper = propUpper;
            info.size = sz;
            info.isReceive = isReceive;
            info.notifyType = "COM_NOTIFY_NONE";
            info.actionInit = "{ .Dummy = NULL }";
            info.receiverCount = receiversForMessage[idx].size();
            info.receivers = receiversForMessage[idx];
            if (info.receiverCount > 0) {
                info.receiverArray = "Com_MessageReceivers_" + sanitize(m->name);
            }
            auto filterAttr = findAttr(*m, "FILTER");
            std::string filterTok = "ALWAYS";
            if (filterAttr && !filterAttr->empty()) {
                auto end = filterAttr->find_first_of(" {");
                filterTok = toUpper(filterAttr->substr(0, end));
            }
            auto setFilter = [&](const std::string& tok) {
                if (tok == "NEVER") info.filterAlg = 1;
                else if (tok == "MASKEDNEWEQUALSX") info.filterAlg = 2;
                else if (tok == "MASKEDNEWDIFFERSX") info.filterAlg = 3;
                else if (tok == "NEWISEQUAL") info.filterAlg = 4;
                else if (tok == "NEWISDIFFERENT") info.filterAlg = 5;
                else if (tok == "MASKEDNEWEQUALSMASKEDOLD") info.filterAlg = 6;
                else if (tok == "MASKEDNEWDIFFERSMASKEDOLD") info.filterAlg = 7;
                else if (tok == "NEWISWITHIN") info.filterAlg = 8;
                else if (tok == "NEWISOUTSIDE") info.filterAlg = 9;
                else if (tok == "NEWISGREATER") info.filterAlg = 10;
                else if (tok == "NEWISLESSOREQUAL") info.filterAlg = 11;
                else if (tok == "NEWISLESS") info.filterAlg = 12;
                else if (tok == "NEWISGREATEROREQUAL") info.filterAlg = 13;
                else if (tok == "ONEEVERYN") info.filterAlg = 14;
                else info.filterAlg = 0;
            };
            setFilter(filterTok);
            auto setNum = [&](uint64_t& target, const std::optional<std::string>& src, const std::string& key) {
                auto val = parseNumericToken(src, key);
                if (val) target = *val;
            };
            setNum(info.mask, filterAttr, "MASK");
            setNum(info.x, filterAttr, "X");
            setNum(info.min, filterAttr, "MIN");
            setNum(info.max, filterAttr, "MAX");
            setNum(info.period, filterAttr, "PERIOD");
            setNum(info.offset, filterAttr, "OFFSET");
            auto initAttr = findAttr(*m, "INITIALVALUE");
            if (!initAttr) initAttr = findAttr(*m, "MESSAGEPROPERTY");
            setNum(info.initialValue, initAttr, "INITIALVALUE");
            auto queueAttr = findAttr(*m, "QUEUESIZE");
            if (!queueAttr) queueAttr = findAttr(*m, "MESSAGEPROPERTY");
            if (queueAttr) {
                info.queueSize = parseNumberToken(queueAttr, "QUEUESIZE").value_or(0);
            }
            if (propUpper.find("QUEUED") != std::string::npos) info.isQueued = true;
            auto netAttr = findAttr(*m, "NETWORKMESSAGE");
            if (!netAttr) netAttr = parseFirstToken(findAttr(*m, "MESSAGEPROPERTY"), "NETWORKMESSAGE");
            auto linkNet = parseFirstToken(findAttr(*m, "LINK"), "NETWORKMESSAGE");
            if (!netAttr && linkNet) netAttr = linkNet;
            if (netAttr) {
                auto it = networkMessageIndex.find(*netAttr);
                if (it != networkMessageIndex.end()) info.networkId = static_cast<int16_t>(it->second);
                else {
                    auto upper = toUpper(*netAttr);
                    for (const auto& kv : networkMessageIndex) {
                        if (toUpper(kv.first) == upper) {
                            info.networkId = static_cast<int16_t>(kv.second);
                            break;
                        }
                    }
                }
            }
            auto linkAttr = findAttr(*m, "LINK");
            if (!linkAttr) linkAttr = findAttr(*m, "RECEIVEMESSAGE");
            auto linkMsg = parseFirstToken(linkAttr, "RECEIVEMESSAGE");
            if (linkMsg) {
                info.linkEnabled = true;
                auto it = messageIndex.find(*linkMsg);
                if (it != messageIndex.end()) info.linkMessage = static_cast<uint16_t>(it->second);
                else {
                    auto upper = toUpper(*linkMsg);
                    for (const auto& kv : messageIndex) {
                        if (toUpper(kv.first) == upper) {
                            info.linkMessage = static_cast<uint16_t>(kv.second);
                            break;
                        }
                    }
                }
            }
            auto notif = findAttr(*m, "NOTIFICATION");
            std::string notifUpper = notif ? toUpper(*notif) : "";
            if (notif) {
                auto taskName = parseFirstToken(*notif, "TASK");
                if (!taskName) {
                    auto directTask = findAttr(*m, "TASK");
                    if (directTask) taskName = *directTask;
                }
                uint8_t resolvedTaskId = resolveTaskId(taskName);
                if (resolvedTaskId == 0) {
                    std::regex re(R"(TASK\s*=\s*([A-Za-z_]\w*))", std::regex::icase);
                    std::sregex_iterator it(notif->begin(), notif->end(), re);
                    std::sregex_iterator end;
                    for (; it != end; ++it) {
                        auto candidate = (*it)[1].str();
                        resolvedTaskId = resolveTaskId(std::optional<std::string>(candidate));
                        if (resolvedTaskId != 0) break;
                    }
                }
                if (notifUpper.find("ACTIVATETASK") != std::string::npos) {
                    info.notifyType = "COM_ACTIVATETASK";
                    info.actionInit = "{ .TaskID = (TaskType)" + std::to_string(resolvedTaskId) + " }";
                } else if (notifUpper.find("SETEVENT") != std::string::npos) {
                    info.notifyType = "COM_SETEVENT";
                    uint32_t maskVal = 0;
                    auto maskToken = parseNumberToken(*notif, "MASK");
                    if (maskToken) {
                        maskVal = *maskToken;
                    } else {
                        auto eventToken = parseFirstToken(*notif, "EVENT");
                        maskVal = resolveEventMask(eventToken);
                    }
                    std::string setEvtName = "Com_MessageSetEvent_" + sanitize(m->name);
                    std::ostringstream fwd;
                    fwd << "static Com_MessageSetEventType " << setEvtName << ";";
                    info.setEventForward = fwd.str();
                    std::ostringstream def;
                    def << "static Com_MessageSetEventType " << setEvtName << " = { (TaskType)" << static_cast<unsigned>(resolvedTaskId)
                        << ", (EventMaskType)" << maskVal << " };";
                    info.setEventDef = def.str();
                    info.actionInit = "{ .Event = &" + setEvtName + " }";
                } else if (notifUpper.find("FLAG") != std::string::npos) {
                    info.notifyType = "COM_FLAG";
                    info.actionInit = "{ .Flag = (COMFlagType)0 }";
                }
            }
            messageInfos.push_back(info);
        }
        c << "\n";
        for (const auto& info : messageInfos) {
            if (!info.setEventForward.empty()) {
                c << info.setEventForward << "\n";
            }
        }
        for (size_t i = 0; i < messageInfos.size(); ++i) {
            if (messageInfos[i].receiverCount == 0) continue;
            c << "static const Com_ReceiverType " << messageInfos[i].receiverArray << "[" << messageInfos[i].receiverCount << "] = {";
            for (size_t r = 0; r < messageInfos[i].receivers.size(); ++r) {
                auto ridx = messageInfos[i].receivers[r];
                c << " { (MessageIdentifier)MESSAGE_ID_" << sanitize(messages[ridx]->name) << " }";
                if (r + 1 < messageInfos[i].receivers.size()) c << ", ";
            }
            c << " };\n";
        }
        c << "const Com_MessageObjectType Com_MessageObjects[COM_NUMBER_OF_MESSAGES] = {\n";
        for (size_t i = 0; i < messageInfos.size(); ++i) {
            const auto& info = messageInfos[i];
            c << "    { " << info.propUpper << ", " << info.notifyType << ", " << info.actionInit << ", (uint8)" << info.size << ", ";
            if (info.isReceive) {
                c << "(const ApplicationDataRef *)&Com_MessageData_" << sanitize(messages[i]->name) << ", (uint8)" << info.receiverCount << ", ";
            } else {
                c << "(const ApplicationDataRef *)NULL, (uint8)" << info.receiverCount << ", ";
            }
            if (info.receiverCount > 0) {
                c << "(const Com_ReceiverType *)" << info.receiverArray;
            } else {
                c << "(const Com_ReceiverType *)NULL";
            }
            c << ", (Com_AddressInformation const * const)NULL }";
            if (i + 1 < messageInfos.size()) c << ",";
            c << "\n";
        }
        c << "};\n\n";
        c << "const Os_MessageFilterConfig Os_MessageFilters[COM_NUMBER_OF_MESSAGES] = {\n";
        for (size_t i = 0; i < messageInfos.size(); ++i) {
            const auto& info = messageInfos[i];
            std::string linkId = "0xFFFF";
            if (info.linkMessage != 0xFFFF && info.linkMessage < messages.size()) {
                linkId = "(uint16_t)MESSAGE_ID_" + sanitize(messages[info.linkMessage]->name);
            }
            c << "    { (Com_FilterAlgorithm)" << static_cast<unsigned>(info.filterAlg) << ", "
              << static_cast<unsigned long long>(info.mask) << ", "
              << static_cast<unsigned long long>(info.x) << ", "
              << static_cast<unsigned long long>(info.min) << ", "
              << static_cast<unsigned long long>(info.max) << ", "
              << static_cast<unsigned long long>(info.period) << ", "
              << static_cast<unsigned long long>(info.offset) << ", "
              << static_cast<unsigned long long>(info.initialValue) << ", "
              << static_cast<unsigned long long>(info.queueSize) << ", "
              << linkId << ", "
              << (info.linkEnabled ? 1 : 0) << ", "
              << (info.isQueued ? 1 : 0) << " }";
            if (i + 1 < messageInfos.size()) c << ",";
            c << "\n";
        }
        c << "};\n\n";
        c << "const int16_t Os_MessageNetworkId[COM_NUMBER_OF_MESSAGES] = {";
        for (size_t i = 0; i < messageInfos.size(); ++i) {
            c << messageInfos[i].networkId;
            if (i + 1 < messageInfos.size()) c << ", ";
        }
        c << "};\n\n";
        for (const auto& info : messageInfos) {
            if (!info.setEventDef.empty()) {
                c << info.setEventDef << "\n";
            }
        }
        c << "\n";
    }

    if (!networkMessages.empty()) {
        c << "const Os_NetworkMessageConfig Os_NetworkMessages[OS_NUMBER_OF_NETWORKMESSAGES] = {\n";
        for (size_t i = 0; i < networkMessages.size(); ++i) {
            auto prop = firstTokenUpper(findAttr(*networkMessages[i], "MESSAGEPROPERTY"));
            auto ipduRef = findAttr(*networkMessages[i], "IPDU");
            int16_t ipduId = -1;
            if (ipduRef) {
                auto it = ipduIndex.find(*ipduRef);
                if (it != ipduIndex.end()) ipduId = static_cast<int16_t>(it->second);
                else {
                    auto upper = toUpper(*ipduRef);
                    for (const auto& kv : ipduIndex) {
                        if (toUpper(kv.first) == upper) {
                            ipduId = static_cast<int16_t>(kv.second);
                            break;
                        }
                    }
                }
            }
            uint32_t sizeBits = parseNumericToken(findAttr(*networkMessages[i], "SIZEINBITS"), "SIZEINBITS").value_or(0);
            uint32_t maxSizeBits = parseNumericToken(findAttr(*networkMessages[i], "MAXIMUMSIZEINBITS"), "MAXIMUMSIZEINBITS").value_or(sizeBits);
            uint32_t bitPos = parseNumericToken(findAttr(*networkMessages[i], "BITPOSITION"), "BITPOSITION").value_or(0);
            uint64_t initial = parseNumericToken(findAttr(*networkMessages[i], "INITIALVALUE"), "INITIALVALUE").value_or(0);
            auto bitOrder = findAttr(*networkMessages[i], "BITORDERING").value_or("");
            auto interpretation = findAttr(*networkMessages[i], "DATAINTERPRETATION").value_or("");
            auto direction = firstTokenUpper(findAttr(*networkMessages[i], "DIRECTION"));
            auto transfer = firstTokenUpper(findAttr(*networkMessages[i], "TRANSFERPROPERTY"));
            c << "    { " << ipduId << ", \"" << prop << "\", " << sizeBits << ", " << maxSizeBits << ", " << bitPos
              << ", \"" << bitOrder << "\", \"" << interpretation << "\", \"" << direction << "\", \"" << transfer << "\", "
              << static_cast<unsigned long long>(initial) << " }";
            if (i + 1 < networkMessages.size()) c << ",";
            c << "\n";
        }
        c << "};\n\n";
    }

    if (!ipdus.empty()) {
        c << "const Os_IpduConfig Os_Ipdus[OS_NUMBER_OF_IPDUS] = {\n";
        for (size_t i = 0; i < ipdus.size(); ++i) {
            auto property = firstTokenUpper(findAttr(*ipdus[i], "IPDUPROPERTY"));
            auto transmission = firstTokenUpper(findAttr(*ipdus[i], "TRANSMISSIONMODE"));
            uint32_t sizeBits = parseNumericToken(findAttr(*ipdus[i], "SIZEINBITS"), "SIZEINBITS").value_or(0);
            auto period = parseNumericToken(findAttr(*ipdus[i], "TRANSMISSIONMODE"), "TIMEPERIOD").value_or(0);
            auto offset = parseNumericToken(findAttr(*ipdus[i], "TRANSMISSIONMODE"), "TIMEOFFSET").value_or(0);
            auto minDelay = parseNumericToken(findAttr(*ipdus[i], "TRANSMISSIONMODE"), "MINIMUMDELAYTIME").value_or(0);
            auto timeout = parseNumericToken(findAttr(*ipdus[i], "TIMEOUT"), "TIMEOUT").value_or(0);
            auto firstTimeout = parseNumericToken(findAttr(*ipdus[i], "FIRSTTIMEOUT"), "FIRSTTIMEOUT").value_or(0);
            auto callout = findAttr(*ipdus[i], "IPDUCALLOUT").value_or("");
            auto layer = findAttr(*ipdus[i], "LAYERUSED").value_or("");
            c << "    { \"" << property << "\", \"" << transmission << "\", " << sizeBits << ", "
              << static_cast<unsigned long long>(period) << ", "
              << static_cast<unsigned long long>(offset) << ", "
              << static_cast<unsigned long long>(minDelay) << ", "
              << static_cast<unsigned long long>(timeout) << ", "
              << static_cast<unsigned long long>(firstTimeout) << ", "
              << "\"" << callout << "\", \"" << layer << "\" }";
            if (i + 1 < ipdus.size()) c << ",";
            c << "\n";
        }
        c << "};\n\n";
    }

    emitStringArray(c, "Os_CounterNames", counters);
    c << '\n';
    c << "const Os_CounterConfig Os_Counters[OS_NUMBER_OF_COUNTERS] = {\n";
    for (size_t i = 0; i < counters.size(); ++i) {
        std::string upper = toUpper(counters[i]->name);
        c << "    { OSMAXALLOWEDVALUE_" << upper << ", OSTICKSPERBASE_" << upper << ", OSMINCYCLE_" << upper << " }";
        if (i + 1 < counters.size()) c << ",";
        c << "\n";
    }
    c << "};\n\n";

    if (!events.empty()) {
        emitStringArray(c, "Os_EventNames", events);
        c << '\n';
        c << "const EventMaskType Os_EventMask[OS_NUMBER_OF_EVENTS] = {";
        for (size_t i = 0; i < events.size(); ++i) {
            c << "(EventMaskType)(1u << " << i << ")";
            if (i + 1 < events.size()) c << ", ";
        }
        c << "};\n\n";
    }
    if (!resources.empty()) {
        emitStringArray(c, "Os_ResourceNames", resources);
        c << '\n';
        c << "const char* const Os_ResourceProperty[OS_NUMBER_OF_RESOURCES] = {";
        for (size_t i = 0; i < resources.size(); ++i) {
            auto prop = findAttr(*resources[i], "RESOURCEPROPERTY").value_or("");
            c << "\"" << prop << "\"";
            if (i + 1 < resources.size()) c << ", ";
        }
        c << "};\n\n";
        c << "const Os_ResourceConfigurationType OS_ResourceConf[OS_NUMBER_OF_RESOURCES] = {\n";
        for (size_t i = 0; i < resources.size(); ++i) {
            auto ceil = resourceCeil[i];
            if (ceil == 0) ceil = toNumber(findAttr(*resources[i], "PRIORITY"), 0);
            c << "    { (PriorityType)" << ceil << " }";
            if (i + 1 < resources.size()) c << ",";
            c << "\n";
        }
        c << "};\n\n";
        c << "Os_ResourceType Os_Resources[OS_NUMBER_OF_RESOURCES];\n\n";
    }
    if (hasInternalResourceFeature) {
        c << "const Os_ResourceConfigurationType OS_IntResourceConf[OS_NUMBER_OF_INT_RESOURCES] = {\n";
        if (hasSchedulerResource) {
            c << "    { PRIO_SCHEDULER }";
            if (!internalResources.empty()) c << ",";
            c << "\n";
        }
        for (size_t i = 0; i < internalResources.size(); ++i) {
            const auto* res = internalResources[i];
            uint32_t ceil = 0;
            auto it = resourceIndex.find(res->name);
            if (it != resourceIndex.end()) {
                auto idx = it->second;
                if (idx < resourceCeil.size()) ceil = resourceCeil[idx];
            }
            if (ceil == 0) {
                ceil = toNumber(findAttr(*res, "PRIORITY"), 0);
            }
            c << "    { (PriorityType)" << ceil << " }";
            if (i + 1 < internalResources.size()) c << ",";
            c << "\n";
        }
        c << "};\n\n";
    }
    if (!appmodes.empty()) {
        emitStringArray(c, "Os_AppModeNames", appmodes);
        c << '\n';
    }
    if (!messages.empty()) {
        emitStringArray(c, "Os_MessageNames", messages);
    }
    if (!networkMessages.empty()) {
        emitStringArray(c, "Os_NetworkMessageNames", networkMessages);
    }
    if (!ipdus.empty()) {
        emitStringArray(c, "Os_IpduNames", ipdus);
    }
}

void Generator::writeOrti(const OilModel& model, const Options& opts) const {
    auto out = opts.output_prefix;
    out.replace_extension(".ort");
    std::ofstream o(out, std::ios::binary);
    if (!o) throw std::runtime_error("Cannot open output file: " + out.string());

    auto tasks = filter(model, "TASK");
    auto alarms = filter(model, "ALARM");
    auto counters = filter(model, "COUNTER");
    auto resources = filter(model, "RESOURCE");
    auto appmodes = filter(model, "APPMODE");
    auto isrs = filter(model, "ISR");
    auto messages = filter(model, "MESSAGE");
    std::vector<const ObjectInstance*> autostartTasks;
    std::vector<const ObjectInstance*> otherTasks;
    for (const auto* t : tasks) {
        if (toBool(findAttr(*t, "AUTOSTART"), false)) {
            autostartTasks.push_back(t);
        } else {
            otherTasks.push_back(t);
        }
    }
    std::vector<const ObjectInstance*> orderedTasks;
    orderedTasks.insert(orderedTasks.end(), autostartTasks.begin(), autostartTasks.end());
    orderedTasks.insert(orderedTasks.end(), otherTasks.begin(), otherTasks.end());
    auto priorityOrder = orderedPriorities(orderedTasks);

    std::vector<std::pair<std::string, int>> serviceIds = {
        {"NoService", 0},           {"ActivateTask", 2},      {"TerminateTask", 4},     {"ChainTask", 6},
        {"Schedule", 8},            {"GetTaskID", 10},        {"GetTaskState", 12},     {"DisableAllInterrupts", 14},
        {"EnableAllInterrupts", 16},{"SuspendAllInterrupts", 18},{"ResumeAllInterrupts", 20},{"SuspendOSInterrupts", 22},
        {"ResumeOSInterrupts", 24}, {"GetResource", 26},      {"ReleaseResource", 28},  {"SetEvent", 30},
        {"ClearEvent", 32},         {"GetEvent", 34},         {"WaitEvent", 36},        {"GetAlarmBase", 38},
        {"GetAlarm", 40},           {"SetRelAlarm", 42},      {"SetAbsAlarm", 44},      {"CancelAlarm", 46},
        {"GetActiveApplicationMode", 48}, {"StartOS", 50},    {"ShutdownOS", 52},       {"GetApplicationID", 54},
        {"GetISRID", 56},           {"CallTrustedFunction", 58},{"CheckISRMemoryAccess", 60},{"CheckTaskMemoryAccess", 62},
        {"CheckObjectAccess", 64},  {"CheckObjectOwnership", 66},{"StartScheduleTableRel", 68},{"StartScheduleTableAbs", 70},
        {"StopScheduleTable", 72},  {"NextScheduleTable", 74},{"StartScheduleTableSynchron", 76},{"SyncScheduleTable", 78},
        {"GetScheduleTableStatus", 80},{"SetScheduleTableAsync", 82},{"TerminateApplication", 84},{"DisableInterruptSource", 86},
        {"EnableInterruptSource", 88},{"InitCounter", 90},    {"IncrementCounter", 92}, {"GetCounterInfo", 94},
        {"GetCounterValue", 96},    {"GetElapsedCounterValue", 98},{"StartCOM", 100},   {"StopCOM", 102},
        {"InitMessage", 104},       {"StartPeriodic", 106},   {"StopPeriodic", 108},    {"SendMessage", 110},
        {"ReceiveMessage", 112},    {"SendDynamicMessage", 114},{"ReceiveDynamicMessage", 116},{"SendZeroMessage", 118},
        {"GetMessageStatus", 120}
    };

    std::vector<std::pair<std::string, int>> statusTypes = {
        {"E_OK", 0}, {"E_OS_ACCESS", 1}, {"E_OS_CALLEVEL", 2}, {"E_OS_ID", 3}, {"E_OS_LIMIT", 4}, {"E_OS_NOFUNC", 5},
        {"E_OS_RESOURCE", 6}, {"E_OS_STATE", 7}, {"E_OS_VALUE", 8}, {"E_OS_SYS_ASSERTION_FAILED", 9},
        {"E_OS_SERVICEID", 16}, {"E_OS_ILLEGAL_ADDRESS", 17}, {"E_OS_MISSINGEND", 18}, {"E_OS_DISABLEDINT", 19},
        {"E_OS_STACKFAULT", 20}, {"E_OS_PROTECTION_MEMORY", 21}, {"E_OS_PROTECTION_TIME", 22}, {"E_OS_PROTECTION_ARRIVAL", 23},
        {"E_OS_PROTECTION_LOCKED", 24}, {"E_OS_PROTECTION_EXCEPTION", 25}, {"E_COM_ID", 32}, {"E_COM_LENGTH", 33},
        {"E_COM_LIMIT", 34}, {"E_COM_NOMSG", 35}, {"E_COM_SYS_UNINIT", 48}
    };

    o << "/* Auto-generated by kosgen_cpp */\n\n";
    o << "VERSION {\n";
    o << "    KOIL        = \"" << model.oil_version << "\";\n";
    o << "    OSSEMANTICS = \"ORTI\", \"2.2\";\n";
    o << "};\n\n";

    o << "IMPLEMENTATION K_OS_0_9_0_ORTI {\n";
    o << "    OS {\n";
    o << "        TOTRACE ENUM \"uint8\" [\n";
    for (size_t i = 0; i < serviceIds.size(); ++i) {
        o << "            \"" << serviceIds[i].first << "\" = " << serviceIds[i].second;
        o << (i + 1 < serviceIds.size() ? "," : "") << "\n";
    }
    o << "        ] SERVICETRACE, \"OS Service Watches\";\n\n";

    o << "        ENUM \"uint8\" [\n";
    o << "            \"NONE\" = 0";
    for (size_t i = 0; i < priorityOrder.size(); ++i) {
        o << ",\n            \"" << priorityOrder[i] << "\" = " << legacyPriorityValue(i + 1);
    }
    o << "\n        ] RUNNINGTASKPRIORITY, \"Priority of running Task\";\n\n";

    o << "        ENUM \"uint8\" [\n";
    o << "            \"NONE\" = 0";
    for (size_t i = 0; i < isrs.size(); ++i) {
        o << ",\n            \"" << sanitize(isrs[i]->name) << "\" = " << (i + 1);
    }
    o << "\n        ] RUNNINGISR2, \"Running ISR of category 2\";\n\n";

    o << "        ENUM \"uint8\" [\n";
    for (size_t i = 0; i < statusTypes.size(); ++i) {
        o << "            \"" << statusTypes[i].first << "\" = " << statusTypes[i].second;
        o << (i + 1 < statusTypes.size() ? "," : "") << "\n";
    }
    o << "        ] LASTERROR, \"Last OSEK error\";\n\n";

    o << "        ENUM [\n";
    o << "            \"IdleTask\" = \"0\"";
    for (size_t i = 0; i < orderedTasks.size(); ++i) {
        o << ",\n            \"" << orderedTasks[i]->name << "\" = \"" << (i + 1) << "\"";
    }
    o << "\n        ] RUNNINGTASK, \"Running Task Identification\";\n\n";

    o << "        ENUM \"uint8\" [\n";
    o << "            \"DEFAULT APPMODE\" = 0";
    for (size_t i = 0; i < appmodes.size(); ++i) {
        o << ",\n            \"" << appmodes[i]->name << "\" = " << (i + 1);
    }
    o << "\n        ] CURRENTAPPMODE, \"Current application mode\";\n";
    o << "    }, \"OS\";\n\n";

    if (!messages.empty()) {
        o << "    MESSAGE {\n";
        o << "        ENUM \"uint16\" [\n";
        for (size_t i = 0; i < messages.size(); ++i) {
            o << "            \"" << sanitize(messages[i]->name) << "\" = " << i;
            if (i + 1 < messages.size()) o << ",";
            o << "\n";
        }
        o << "        ] CURRENTMESSAGE, \"Current COM message parameter\";\n";
        o << "    }, \"COM\";\n\n";
    }

    o << "    TASK {\n";
    o << "        ENUM \"uint8\" [\n";
    o << "            \"NONE\" = 0";
    for (size_t i = 0; i < priorityOrder.size(); ++i) {
        o << ",\n            \"" << priorityOrder[i] << "\" = " << legacyPriorityValue(i + 1);
    }
    o << "\n        ] PRIORITY, \"Actual Priority\";\n";
    o << "        ENUM \"uint8\" [\n";
    o << "            \"SUSPENDED\" = 0,\n";
    o << "            \"READY\"     = 1,\n";
    o << "            \"RUNNING\"   = 2,\n";
    o << "            \"WAITING\"   = 3\n";
    o << "        ] STATE, \"Current Task State\";\n";
    o << "        ENUM \"uint8 *\" [\n";
    bool firstStack = true;
    for (size_t i = 0; i < orderedTasks.size(); ++i) {
        o << (firstStack ? "            " : ",\n            ") << "\"Stack" << i << "\" : " << sanitize(orderedTasks[i]->name)
          << "_Stack = \"&(" << sanitize(orderedTasks[i]->name) << "_Stack[0])\"";
        firstStack = false;
    }
    o << "\n        ] STACK, \"Task Stack\";\n";
    o << "        ENUM \"uint8 *\" [\n";
    bool firstCtx = true;
    for (size_t i = 0; i < orderedTasks.size(); ++i) {
        o << (firstCtx ? "            " : ",\n            ") << "\"CONTEXT_" << sanitize(orderedTasks[i]->name) << "\" : CONTEXT_" << sanitize(orderedTasks[i]->name)
          << " = " << i;
        firstCtx = false;
    }
    o << "\n        ] CONTEXT, \"Task Context\";\n";
    o << "        CTYPE \"uint8\" CURRENTACTIVATIONS, \"Current Activations\";\n";
    o << "        STRING vs_Base_Priority, \"Configured Base Priority\";\n";
    o << "        STRING vs_Schedule, \"Schedule\";\n";
    o << "        STRING vs_Task_Type, \"Task Type\";\n";
    o << "        CTYPE \"uint8\" vs_Events_Set, \"Events Set\";\n";
    o << "        CTYPE \"uint8\" vs_Events_WaitingFor, \"Events Waiting for\";\n";
    o << "    }, \"Tasks\";\n\n";

    o << "    vs_COUNTER {\n";
    o << "        CTYPE \"TickType\" vs_MAXALLOWEDVALUE, \"Maximum possible allowed count value\";\n";
    o << "        CTYPE \"TickType\" vs_MINCYCLE, \"Minimum allowed number of ticks for a cyclic alarm\";\n";
    o << "        CTYPE \"TickType\" vs_TICKSPERBASE, \"Number of ticks required to reach a counter-specific\";\n";
    o << "        STRING vs_TYPE, \"Type of counter\";\n";
    o << "        CTYPE \"TickType\" vs_CURRENTVALUE, \"Current Value\";\n";
    o << "    }, \"Counters\";\n\n";

    o << "    ALARM {\n";
    o << "        CTYPE \"TickType\" ALARMTIME, \"Alarm Time\";\n";
    o << "        CTYPE \"TickType\" CYCLETIME, \"Cycle Time\";\n";
    o << "        ENUM [\n";
    o << "            \"STOPPED\" = 0,\n";
    o << "            \"RUNNING\" = 1\n";
    o << "        ] STATE, \"Alarm State\";\n";
    o << "        STRING ACTION, \"associated Action\";\n";
    o << "        STRING COUNTER, \"associated Counter\";\n";
    o << "    }, \"Alarms\";\n\n";

    o << "    RESOURCE {\n";
    o << "        ENUM \"uint8\" [\n";
    o << "            \"UNLOCKED\"=0,\n";
    o << "            \"LOCKED\"=1\n";
    o << "        ] STATE, \"Resource State\";\n";
    o << "        ENUM \"uint8\" [\n";
    o << "            \"NONE\" = 0";
    for (size_t i = 0; i < orderedTasks.size(); ++i) {
        o << ",\n            \"" << orderedTasks[i]->name << "\"=" << (i + 1);
    }
    o << "\n        ] LOCKER, \"Resource Locker\";\n";
    o << "        STRING PRIORITY, \"Ceiling priority\";\n";
    o << "    }, \"Resources\";\n\n";

    o << "    CONTEXT {\n";
    o << "        CTYPE \"uint16\" _CPU_PC, \"CPU Register PC\";\n";
    o << "        CTYPE \"uint16\" _CPU_Y, \"CPU Register Y\";\n";
    o << "        CTYPE \"uint16\" _CPU_X, \"CPU Register X\";\n";
    o << "        CTYPE \"uint16\" _CPU_D, \"CPU Register D\";\n";
    o << "        CTYPE \"uint8\" _CPU_A, \"CPU Register A\";\n";
    o << "        CTYPE \"uint8\" _CPU_B, \"CPU Register B\";\n";
    o << "        CTYPE \"uint8\" _CPU_CCR, \"CPU Register CCR\";\n";
    o << "        CTYPE \"unsigned char\" VALID;\n";
    o << "    }, \"Task Context\";\n\n";

    o << "    STACK {\n";
    o << "        CTYPE \"uint16\" SIZE, \"Stacksize in Bytes\";\n";
    o << "        CTYPE \"uint16*\" BASEADDRESS, \"Base Address\";\n";
    o << "        STRING STACKDIRECTION, \"Stack Direction\";\n";
    o << "        CTYPE \"uint8\" FILLPATTERN, \"Stack Fill Pattern\";\n";
    o << "    }, \"Stacks\";\n";
    o << "}; /* END OF IMPLEMENTATION */\n\n";

    o << "OS K_OS_0_9_0_arch {\n";
    o << "    RUNNINGTASK         = \"(uint8)OsCurrentTID\";\n";
    o << "    RUNNINGTASKPRIORITY = \"(OS_TaskConf[OsCurrentTID].Priority)\";\n";
    o << "    RUNNINGISR2         = \"(uint8)OsCurrentISRID\";\n";
    o << "    SERVICETRACE        = \"Os_ServiceContext.id\";\n";
    o << "    LASTERROR           = \"OsLastError\";\n";
    o << "    CURRENTAPPMODE      = \"Os_AppMode\";\n";
    o << "};\n\n";

    if (!messages.empty()) {
        o << "MESSAGE COM {\n";
        o << "    CURRENTMESSAGE      = \"(MessageIdentifier)(Os_ServiceContext.param1)\";\n";
        o << "};\n\n";
    }

    for (size_t i = 0; i < orderedTasks.size(); ++i) {
        auto basePrio = toNumber(findAttr(*orderedTasks[i], "PRIORITY"), 1);
        auto sched = findAttr(*orderedTasks[i], "SCHEDULE").value_or("NON");
        auto typeAttr = findAttr(*orderedTasks[i], "TYPE").value_or("EXTENDED");
        std::string type = typeAttr;
        o << "TASK " << sanitize(orderedTasks[i]->name) << " {\n";
        o << "    PRIORITY            = \"(OS_TCB[" << (i + 1) << "].CurrentPriority)\";\n";
        o << "    STATE               = \"(OS_TCB[" << (i + 1) << "].State)\";\n";
        o << "    STACK               = \"&(" << sanitize(orderedTasks[i]->name) << "_Stack[0])\";\n";
        o << "    CURRENTACTIVATIONS  = \"(OS_TCB[" << (i + 1) << "].Activations)\";\n";
        o << "    CONTEXT             = \"" << i << "\";\n";
        o << "    vs_Base_Priority    = \"" << basePrio << "\";\n";
        o << "    vs_Schedule         = \"" << sched << "\";\n";
        o << "    vs_Task_Type        = \"" << type << "\";\n";
        o << "    vs_Events_WaitingFor= \"(OS_TCB[" << (i + 1) << "].EventsWaitingFor)\";\n";
        o << "    vs_Events_Set       = \"(OS_TCB[" << (i + 1) << "].EventsSet)\";\n";
        o << "};\n\n";

        o << "STACK " << sanitize(orderedTasks[i]->name) << "_Stack {\n";
        o << "    SIZE                = \"(OS_TaskConf[" << (i + 1) << "].StackSize)\";\n";
        o << "    STACKDIRECTION      = \"DOWN\";\n";
        o << "    BASEADDRESS         = \"&(" << sanitize(orderedTasks[i]->name) << "_Stack[0])\";\n";
        o << "    FILLPATTERN         = \"0\";\n";
        o << "};\n\n";

        o << "CONTEXT CONTEXT_" << sanitize(orderedTasks[i]->name) << " {\n";
        o << "    _CPU_PC = \"*(uint16 *)((uint8*)(OS_TCB[" << (i + 1) << "].Stackpointer + 7))\";\n";
        o << "    _CPU_Y = \"*(uint16 *)((uint8*)(OS_TCB[" << (i + 1) << "].Stackpointer + 5))\";\n";
        o << "    _CPU_X = \"*(uint16 *)((uint8*)(OS_TCB[" << (i + 1) << "].Stackpointer + 3))\";\n";
        o << "    _CPU_D = \"(uint16 ) ( (*(OS_TCB[" << (i + 1) << "].Stackpointer + 1)) | (*(OS_TCB[" << (i + 1) << "].Stackpointer + 2) << 8 ) )\";\n";
        o << "    _CPU_A = \"*(uint8 *)((uint8*)(OS_TCB[" << (i + 1) << "].Stackpointer + 2))\";\n";
        o << "    _CPU_B = \"*(uint8 *)((uint8*)(OS_TCB[" << (i + 1) << "].Stackpointer + 1))\";\n";
        o << "    _CPU_CCR = \"*(uint8 *)((uint8*)(OS_TCB[" << (i + 1) << "].Stackpointer + 0))\";\n";
        o << "    VALID = \"((OS_TCB[" << (i + 1) << "].State!=SUSPENDED) && (OS_TCB[" << (i + 1) << "].State!=RUNNING))\";\n";
        o << "};\n\n";
    }

    for (size_t ci = 0; ci < counters.size(); ++ci) {
        auto maxv = toNumber(findAttr(*counters[ci], "MAXALLOWEDVALUE"), 0);
        auto minc = toNumber(findAttr(*counters[ci], "MINCYCLE"), 0);
        auto tpb = toNumber(findAttr(*counters[ci], "TICKSPERBASE"), 0);
        auto typeAttr = findAttr(*counters[ci], "TYPE").value_or("");
        auto drv = extractDriver(findAttr(*counters[ci], "TYPE"), findAttr(*counters[ci], "DRIVER"));
        if (!drv) drv = findAttr(*counters[ci], "HC12_TYPE");
        if (typeAttr.find("HARDWARE") != std::string::npos && drv && !drv->empty()) {
            typeAttr = "HARDWARE [Driver: " + *drv + "]";
        }
        o << "vs_COUNTER " << sanitize(counters[ci]->name) << " {\n";
        o << "    vs_MAXALLOWEDVALUE  = \"" << maxv << "\";\n";
        o << "    vs_MINCYCLE         = \"" << minc << "\";\n";
        o << "    vs_TICKSPERBASE     = \"" << tpb << "\";\n";
        o << "    vs_CURRENTVALUE     = \"Os_CounterValues[" << ci << "]\";\n";
        o << "    vs_TYPE             = \"" << typeAttr << "\";\n";
        o << "};\n\n";
    }

    for (size_t ai = 0; ai < alarms.size(); ++ai) {
        auto counterName = findAttr(*alarms[ai], "COUNTER").value_or("");
        auto actionRaw = findAttr(*alarms[ai], "ACTION");
        auto actionType = normalizeActionName(parseActionType(actionRaw), serviceIds);
        auto actionTask = parseFirstToken(actionRaw, "TASK").value_or("");
        o << "ALARM " << sanitize(alarms[ai]->name) << " {\n";
        o << "    ALARMTIME = \"OS_AlarmValue[" << ai << "].ExpireCounter\";\n";
        o << "    CYCLETIME = \"OS_AlarmValue[" << ai << "].CycleCounter\";\n";
        o << "    STATE     = \"((OsAlm_ActiveAlarms & (1 << " << ai << "))!=0) ? 1 : 0\";\n";
        o << "    ACTION    = \"" << actionType;
        if (!actionTask.empty()) o << " [" << actionTask << "]";
        o << "\";\n";
        o << "    COUNTER   = \"" << counterName << "\";\n";
        o << "};\n\n";
    }

}

void Generator::writeTargetTemplate(const Options& opts) const {
    if (opts.target.empty()) return;

    auto target = toLower(opts.target);
    auto out_dir = opts.output_prefix.parent_path();
    if (out_dir.empty()) out_dir = ".";
    std::error_code ec;
    std::filesystem::create_directories(out_dir, ec);
    auto out = out_dir / "kosgen.cmake";

    std::ofstream cm(out, std::ios::binary);
    if (!cm) throw std::runtime_error("Cannot open output file: " + out.string());

    auto prefix_name = opts.output_prefix.filename().generic_string();
    if (prefix_name.empty()) prefix_name = "Os_Cfg";

    cm << "# Auto-generated build snippet by kosgen_cpp (target: " << target << ")\n";
    cm << "# Regenerate with kosgen_cpp; manual edits may be overwritten.\n\n";
    cm << "if(NOT DEFINED KOS_SRC_PATH)\n";
    cm << "  if(DEFINED ENV{KOS_SRC_PATH})\n";
    cm << "    set(KOS_SRC_PATH \"$ENV{KOS_SRC_PATH}\")\n";
    cm << "  else()\n";
    cm << "    message(FATAL_ERROR \"KOS_SRC_PATH is not set; point it to the k_os source root.\")\n";
    cm << "  endif()\n";
    cm << "endif()\n\n";

    cm << "set(KOSGEN_TARGET \"" << target << "\")\n";
    cm << "set(KOSGEN_OUTPUT_DIR \"${CMAKE_CURRENT_LIST_DIR}\")\n";
    cm << "set(KOSGEN_OUTPUT_PREFIX \"${KOSGEN_OUTPUT_DIR}/" << prefix_name << "\")\n\n";

    cm << "set(KOSGEN_GENERATED_SOURCES\n";
    cm << "    \"${KOSGEN_OUTPUT_PREFIX}.c\"\n";
    cm << "    \"${KOSGEN_OUTPUT_PREFIX}.h\"\n";
    cm << "    \"${KOSGEN_OUTPUT_PREFIX}.info.txt\"\n";
    if (opts.generate_orti) {
        cm << "    \"${KOSGEN_OUTPUT_PREFIX}.ort\"\n";
    }
    if (opts.generate_main_stub) {
        cm << "    \"${KOSGEN_OUTPUT_DIR}/main.c\"\n";
    }
    cm << ")\n\n";

    cm << "set(KOSGEN_INCLUDE_DIRS\n";
    cm << "    \"${KOS_SRC_PATH}\"\n";
    cm << "    \"${KOS_SRC_PATH}/inc\"\n";
    cm << ")\n\n";

    cm << "set(KOSGEN_KERNEL_SOURCES\n";
    cm << "    \"${KOS_SRC_PATH}/src/Com_Flt.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Com_If.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Com_Int.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Alm.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Ctr.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Error.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Evt.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Exec.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Intr.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Mlq.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Orti.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_ParamAccess.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Res.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_SchT.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Task.c\"\n";
    cm << "    \"${KOS_SRC_PATH}/src/Os_Vars.c\"\n";
    cm << ")\n";
    cm << "if(EXISTS \"${KOS_SRC_PATH}/src/Com_Ext.c\")\n";
    cm << "  list(APPEND KOSGEN_KERNEL_SOURCES \"${KOS_SRC_PATH}/src/Com_Ext.c\")\n";
    cm << "endif()\n\n";

    cm << "set(KOSGEN_PORT_SOURCES \"\")\n";
    cm << "set(KOSGEN_PORT_LIBS \"\")\n";
    cm << "if(KOSGEN_TARGET STREQUAL \"rp2040\")\n";
    cm << "  list(APPEND KOSGEN_INCLUDE_DIRS \"${KOS_SRC_PATH}/port/rp2040\" \"${KOS_SRC_PATH}/samples/template\")\n";
    cm << "  list(APPEND KOSGEN_PORT_SOURCES \"${KOS_SRC_PATH}/port/rp2040/Os_Port_RP2040.c\")\n";
    cm << "  list(APPEND KOSGEN_PORT_LIBS pico_stdlib hardware_spi)\n";
    cm << "elseif(KOSGEN_TARGET STREQUAL \"stm32\")\n";
    cm << "  list(APPEND KOSGEN_INCLUDE_DIRS \"${KOS_SRC_PATH}/port/stm32/gcc\")\n";
    cm << "  list(APPEND KOSGEN_PORT_SOURCES \"${KOS_SRC_PATH}/port/stm32/gcc/Os_Port_stm32.c\")\n";
    cm << "elseif(KOSGEN_TARGET STREQUAL \"windows\")\n";
    cm << "  list(APPEND KOSGEN_INCLUDE_DIRS \"${KOS_SRC_PATH}/port/windows\")\n";
    cm << "  list(APPEND KOSGEN_PORT_SOURCES \"${KOS_SRC_PATH}/port/windows/Os_Port_Win32.c\")\n";
    cm << "  list(APPEND KOSGEN_PORT_LIBS kernel32 user32 winmm)\n";
    cm << "elseif(KOSGEN_TARGET STREQUAL \"pthreads\" OR KOSGEN_TARGET STREQUAL \"posix\")\n";
    cm << "  list(APPEND KOSGEN_INCLUDE_DIRS \"${KOS_SRC_PATH}/port/pthreads\")\n";
    cm << "  list(APPEND KOSGEN_PORT_SOURCES \"${KOS_SRC_PATH}/port/pthreads/Os_Port_Unix.c\")\n";
    cm << "  list(APPEND KOSGEN_PORT_LIBS pthread rt)\n";
    cm << "endif()\n\n";

    cm << "# Example usage:\n";
    cm << "# add_executable(kos_app ${KOSGEN_GENERATED_SOURCES} ${KOSGEN_KERNEL_SOURCES} ${KOSGEN_PORT_SOURCES})\n";
    cm << "# target_include_directories(kos_app PRIVATE ${KOSGEN_INCLUDE_DIRS})\n";
    cm << "# target_link_libraries(kos_app ${KOSGEN_PORT_LIBS})\n";
    cm << "# Adjust toolchain/SDK settings as needed for the selected target.\n";
}

void Generator::writeMainStub(const OilModel& model, const Options& opts) const {
    if (!opts.generate_main_stub) return;

    std::filesystem::path out = opts.output_prefix.parent_path();
    if (out.empty()) out = ".";
    std::error_code ec;
    std::filesystem::create_directories(out, ec);
    out /= "main.c";

    if (std::filesystem::exists(out)) {
        return;  // do not overwrite user main
    }

    auto tasks = filter(model, "TASK");
    auto isrs = filter(model, "ISR");
    auto appmodes = filter(model, "APPMODE");

    std::string appMode = "DefaultAppMode";
    if (!appmodes.empty()) {
        appMode = sanitize(appmodes.front()->name);
    }

    std::ofstream m(out, std::ios::binary);
    if (!m) throw std::runtime_error("Cannot open output file: " + out.string());

    m << "/* Auto-generated stub by kosgen_cpp";
    if (!opts.target.empty()) {
        m << " (target: " << opts.target << ")";
    }
    m << " */\n\n";
    m << "#include \"Os.h\"\n";
    m << "#include \"Os_Cfg.h\"\n\n";

    for (const auto* t : tasks) {
        auto name = sanitize(t->name);
        m << "void " << name << "(void)\n{\n"
          << "    /* TODO: implement task '" << t->name << "' */\n"
          << "}\n\n";
    }

    for (const auto* isr : isrs) {
        auto name = sanitize(isr->name);
        m << "void " << name << "(void)\n{\n"
          << "    /* TODO: implement ISR '" << isr->name << "' */\n"
          << "}\n\n";
    }

    m << "int main(void)\n{\n"
      << "    StartOS(" << appMode << ");\n"
      << "    return 0;\n"
      << "}\n";
}

}  // namespace kos::oilcpp
