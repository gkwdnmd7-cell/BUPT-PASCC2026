#include <iostream>
#include <memory>

#include "semantic_tables.h"

namespace {
bool expect(bool cond, const std::string& name) {
    if (!cond) {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
    std::cout << "[PASS] " << name << std::endl;
    return true;
}
}  // namespace

int main() {
    bool ok = true;

    ScopeStack scopes;
    ok &= expect(scopes.empty(), "scope_stack_starts_empty");

    TableSet* global = scopes.push("global");
    ok &= expect(global != nullptr, "push_global_scope");
    ok &= expect(scopes.size() == 1, "scope_stack_size_after_global_push");

    auto intType = std::make_shared<BasicType>(BasicType::BasicKind::Int);
    auto realType = std::make_shared<BasicType>(BasicType::BasicKind::Real);
    ok &= expect(global->insert("integer", intType), "insert_builtin_integer_type");
    ok &= expect(global->insert("real", realType), "insert_builtin_real_type");
    ok &= expect(!global->insert("integer", intType), "reject_duplicate_type_in_same_scope");

    auto globalVar = std::make_shared<VariableSymbol>("x", intType);
    ok &= expect(global->insert("x", globalVar), "insert_global_variable");
    ok &= expect(!global->insert("x", globalVar), "reject_duplicate_symbol_in_same_scope");

    TableSet* local = scopes.push("local_proc");
    ok &= expect(local != nullptr, "push_local_scope");
    ok &= expect(scopes.size() == 2, "scope_stack_size_after_local_push");

    bool localZone = false;
    auto fromParent = local->searchEntry<VariableSymbol>("x", &localZone);
    ok &= expect(fromParent != nullptr, "search_parent_symbol_from_local_scope");
    ok &= expect(!localZone, "parent_symbol_marked_non_local");

    auto localVar = std::make_shared<VariableSymbol>("y", realType);
    ok &= expect(local->insert("y", localVar), "insert_local_variable");

    auto fromLocal = local->searchEntry<VariableSymbol>("y", &localZone);
    ok &= expect(fromLocal != nullptr, "search_local_symbol");
    ok &= expect(localZone, "local_symbol_marked_local");

    auto typeFromParent = local->searchEntry<BasicType>("integer", &localZone);
    ok &= expect(typeFromParent != nullptr, "search_parent_type_from_local_scope");
    ok &= expect(!localZone, "parent_type_marked_non_local");
    ok &= expect(typeFromParent->basicKind() == BasicType::BasicKind::Int, "parent_type_kind_correct");

    ok &= expect(scopes.pop(), "pop_local_scope");
    ok &= expect(scopes.size() == 1, "scope_stack_size_after_local_pop");

    auto hiddenAfterPop = scopes.current()->searchEntry<VariableSymbol>("y", &localZone);
    ok &= expect(hiddenAfterPop == nullptr, "local_symbol_unreachable_after_scope_pop");

    ok &= expect(scopes.pop(), "pop_global_scope");
    ok &= expect(scopes.empty(), "scope_stack_empty_after_all_pops");
    ok &= expect(!scopes.pop(), "pop_on_empty_scope_stack_returns_false");

    return ok ? 0 : 1;
}
