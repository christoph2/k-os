#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace kos::oilcpp {

struct AttributeValue {
    enum class Kind { kIdentifier, kBoolean, kNumber, kString, kAuto } kind{};
    std::string text;
};

struct Attribute {
    std::string name;
    AttributeValue value;
};

struct ObjectInstance {
    std::string object_type;
    std::string name;
    std::vector<Attribute> attributes;
};

struct ImplementationSpec {
    std::string object_type;
    std::string raw_definition;  // textual definition of the implementation spec
};

struct OilModel {
    std::string oil_version;
    std::string preprocessed_text;
    std::vector<ImplementationSpec> implementation_specs;
    std::vector<ObjectInstance> application_objects;
};

}  // namespace kos::oilcpp
