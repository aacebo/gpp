#include "class.hpp"

namespace value {
    Class::Class(string name, unordered_map<string, Type> members) : Object(ObjectType::Class), name(name), members(members) { }

    string Class::to_string() {
        return "<class " + this->name + ">";
    }

    Instance::Instance(Class* def) : Object(ObjectType::Instance), def(def) { }
    Instance::~Instance() {
        for (auto e : this->values) {
            delete e.second;
        }

        this->values.clear();
    }

    bool Instance::has(string name) {
        return this->values.count(name) > 0;
    }

    Value* Instance::get(string name) {
        if (this->has(name)) {
            return this->values[name];
        }

        return nullptr;
    }

    void Instance::set(string name, Value* value) {
        if (this->has(name)) {
            delete this->values[name];
        }

        this->values[name] = value;
    }

    string Instance::to_string() {
        string s = "{ ";

        for (auto it = this->values.begin(); it != this->values.end(); ++it) {
            s += (it->first + ": " + it->second->as_string());

            if (it != this->values.end()) {
                s += ", ";
            }
        }

        return s + " }";
    }
};
