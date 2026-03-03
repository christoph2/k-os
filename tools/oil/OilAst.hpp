#pragma once
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <cstdint>

namespace oil {

struct Location {
    size_t line{1};
    size_t column{1};
};

struct Value; // fwd

struct Attribute {
    std::string name;
    Value* value{nullptr};
    Location loc;
};

struct Node {
    std::string kind;   // e.g., "IMPLEMENTATION", "APPLICATION", "TASK", "OS", ...
    std::string name;   // identifier after object keyword or empty
    std::vector<Attribute> attributes; // NAME = value; pairs found directly inside
    std::vector<Node*> children;       // nested object blocks
    std::optional<std::string> description; // optional description string
    Location loc;
};

struct ScalarString { std::string v; };
struct ScalarIdent  { std::string v; };
struct ScalarInt    { int64_t v{}; };
struct ScalarBool   { bool v{}; };

struct ListValue { std::vector<Value*> elements; };

struct Value {
    using Variant = std::variant<ScalarString, ScalarIdent, ScalarInt, ScalarBool, ListValue>;
    Variant data;
    Location loc;
};

struct OilFile {
    std::string oilVersion;
    Node* implementation{nullptr};
    Node* application{nullptr};
};

// Simple owner to handle raw pointers without pulling in smart pointers across the tree
struct Arena {
    using Deleter = void(*)(void*);
    std::vector<std::pair<void*, Deleter>> blocks;
    ~Arena() {
        for (auto &b : blocks) {
            if (b.second && b.first) b.second(b.first);
        }
    }
    template <typename T, typename... Args>
    T* make(Args&&... args) {
        T* p = new T{std::forward<Args>(args)...};
        blocks.emplace_back(static_cast<void*>(p), [](void* vp){ delete static_cast<T*>(vp); });
        return p;
    }
};

} // namespace oil
