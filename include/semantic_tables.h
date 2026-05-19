#ifndef SEMANTIC_TABLES_H
#define SEMANTIC_TABLES_H

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

class TypeTemplate {
public:
    enum class Kind {
        Basic,
        Array,
        Record,
    };

    explicit TypeTemplate(Kind kind) : kind_(kind) {}
    virtual ~TypeTemplate() = default;

    Kind kind() const { return kind_; }

private:
    Kind kind_;
};

class BasicType : public TypeTemplate {
public:
    enum class BasicKind {
        Int,
        Real,
        Bool,
        Char,
        None,
    };

    explicit BasicType(BasicKind basicKind) : TypeTemplate(Kind::Basic), basicKind_(basicKind) {}

    BasicKind basicKind() const { return basicKind_; }

private:
    BasicKind basicKind_;
};

class ObjectSymbol {
public:
    explicit ObjectSymbol(std::string name) : name_(std::move(name)) {}
    virtual ~ObjectSymbol() = default;

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

class VariableSymbol : public ObjectSymbol {
public:
    VariableSymbol(std::string name, std::shared_ptr<TypeTemplate> type)
        : ObjectSymbol(std::move(name)), type_(std::move(type)) {}

    const std::shared_ptr<TypeTemplate>& type() const { return type_; }

private:
    std::shared_ptr<TypeTemplate> type_;
};

struct RoutineParameter {
    bool byRef = false;
    std::shared_ptr<BasicType> type;
};

class RoutineSymbol : public ObjectSymbol {
public:
    enum class RoutineKind {
        Procedure,
        Function,
    };

    RoutineSymbol(std::string name, RoutineKind routineKind) : ObjectSymbol(std::move(name)), routineKind_(routineKind) {}

    RoutineKind routineKind() const { return routineKind_; }

    void setParameters(std::vector<RoutineParameter> parameters) {
        parameters_ = std::move(parameters);
    }

    const std::vector<RoutineParameter>& parameters() const { return parameters_; }

    void setReturnType(std::shared_ptr<BasicType> t) { returnType_ = std::move(t); }

    const std::shared_ptr<BasicType>& returnType() const { return returnType_; }

private:
    RoutineKind routineKind_;
    std::vector<RoutineParameter> parameters_;
    std::shared_ptr<BasicType> returnType_;
};

template <typename T>
class SymbolTableTemplate {
public:
    bool insert(const std::string& name, std::shared_ptr<T> symbol) {
        return table_.emplace(name, std::move(symbol)).second;
    }

    std::shared_ptr<T> findName(const std::string& name) const {
        auto it = table_.find(name);
        if (it == table_.end()) {
            return nullptr;
        }
        return it->second;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<T>> table_;
};

using SymbolTable = SymbolTableTemplate<ObjectSymbol>;
using TypeTable = SymbolTableTemplate<TypeTemplate>;

class TableSet {
public:
    TableSet(std::string tag, const TableSet* prevSet) : tag_(std::move(tag)), prevSet_(prevSet) {}

    const std::string& tag() const { return tag_; }

    const TableSet* prevSet() const { return prevSet_; }

    template <typename T>
    bool insert(const std::string& name, std::shared_ptr<T> symbol) {
        if constexpr (std::is_base_of_v<TypeTemplate, T>) {
            return defTypes_.insert(name, std::static_pointer_cast<TypeTemplate>(std::move(symbol)));
        } else if constexpr (std::is_base_of_v<ObjectSymbol, T>) {
            return symbols_.insert(name, std::static_pointer_cast<ObjectSymbol>(std::move(symbol)));
        } else {
            static_assert(sizeof(T) == 0, "Unsupported symbol type for TableSet::insert");
        }
    }

    template <typename T>
    std::shared_ptr<T> searchEntry(const std::string& name, bool* localZone = nullptr) const {
        auto local = searchLocal<T>(name);
        if (local) {
            if (localZone != nullptr) {
                *localZone = true;
            }
            return local;
        }

        if (prevSet_ == nullptr) {
            if (localZone != nullptr) {
                *localZone = false;
            }
            return nullptr;
        }

        auto fromParent = prevSet_->searchEntry<T>(name, nullptr);
        if (localZone != nullptr) {
            *localZone = false;
        }
        return fromParent;
    }

private:
    template <typename T>
    std::shared_ptr<T> searchLocal(const std::string& name) const {
        if constexpr (std::is_base_of_v<TypeTemplate, T>) {
            return std::dynamic_pointer_cast<T>(defTypes_.findName(name));
        } else if constexpr (std::is_base_of_v<ObjectSymbol, T>) {
            return std::dynamic_pointer_cast<T>(symbols_.findName(name));
        } else {
            static_assert(sizeof(T) == 0, "Unsupported symbol type for TableSet::searchLocal");
        }
    }

private:
    std::string tag_;
    SymbolTable symbols_;
    TypeTable defTypes_;
    const TableSet* prevSet_;
};

class ScopeStack {
public:
    bool empty() const { return stack_.empty(); }

    const TableSet* current() const {
        if (stack_.empty()) {
            return nullptr;
        }
        return stack_.back().get();
    }

    TableSet* push(const std::string& tag) {
        const TableSet* prev = current();
        stack_.push_back(std::make_unique<TableSet>(tag, prev));
        return stack_.back().get();
    }

    bool pop() {
        if (stack_.empty()) {
            return false;
        }
        stack_.pop_back();
        return true;
    }

    std::size_t size() const { return stack_.size(); }

private:
    std::vector<std::unique_ptr<TableSet>> stack_;
};

class BuiltinTypePool {
public:
    BuiltinTypePool() {
        integer_ = std::make_shared<BasicType>(BasicType::BasicKind::Int);
        real_ = std::make_shared<BasicType>(BasicType::BasicKind::Real);
        boolean_ = std::make_shared<BasicType>(BasicType::BasicKind::Bool);
        character_ = std::make_shared<BasicType>(BasicType::BasicKind::Char);

        byName_.emplace("integer", integer_);
        byName_.emplace("real", real_);
        byName_.emplace("boolean", boolean_);
        byName_.emplace("char", character_);
    }

    const std::shared_ptr<BasicType>& integer() const { return integer_; }
    const std::shared_ptr<BasicType>& real() const { return real_; }
    const std::shared_ptr<BasicType>& boolean() const { return boolean_; }
    const std::shared_ptr<BasicType>& character() const { return character_; }

    std::shared_ptr<BasicType> findByName(const std::string& name) const {
        auto it = byName_.find(name);
        if (it == byName_.end()) {
            return nullptr;
        }
        return it->second;
    }

    bool installInto(TableSet* tableSet) const {
        if (tableSet == nullptr) {
            return false;
        }

        bool ok = true;
        for (const auto& entry : byName_) {
            ok = tableSet->insert(entry.first, entry.second) && ok;
        }
        return ok;
    }

private:
    std::shared_ptr<BasicType> integer_;
    std::shared_ptr<BasicType> real_;
    std::shared_ptr<BasicType> boolean_;
    std::shared_ptr<BasicType> character_;
    std::unordered_map<std::string, std::shared_ptr<BasicType>> byName_;
};

#endif
