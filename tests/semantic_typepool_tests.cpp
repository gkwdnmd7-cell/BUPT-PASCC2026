#include <iostream>

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

    BuiltinTypePool pool;
    ok &= expect(pool.integer() != nullptr, "builtin_integer_exists");
    ok &= expect(pool.real() != nullptr, "builtin_real_exists");
    ok &= expect(pool.boolean() != nullptr, "builtin_boolean_exists");
    ok &= expect(pool.character() != nullptr, "builtin_char_exists");

    ok &= expect(pool.integer()->basicKind() == BasicType::BasicKind::Int, "builtin_integer_kind");
    ok &= expect(pool.real()->basicKind() == BasicType::BasicKind::Real, "builtin_real_kind");
    ok &= expect(pool.boolean()->basicKind() == BasicType::BasicKind::Bool, "builtin_boolean_kind");
    ok &= expect(pool.character()->basicKind() == BasicType::BasicKind::Char, "builtin_char_kind");

    TableSet global("global", nullptr);
    ok &= expect(pool.installInto(&global), "install_builtin_types_into_table_set");

    bool localZone = false;
    auto tInt = global.searchEntry<BasicType>("integer", &localZone);
    ok &= expect(tInt != nullptr, "search_integer_from_table_set");
    ok &= expect(localZone, "builtin_type_marked_local_in_owner_scope");
    ok &= expect(tInt == pool.integer(), "integer_pointer_identity_matches_pool");

    auto tReal = global.searchEntry<BasicType>("real", &localZone);
    ok &= expect(tReal == pool.real(), "real_pointer_identity_matches_pool");

    auto tBool = global.searchEntry<BasicType>("boolean", &localZone);
    ok &= expect(tBool == pool.boolean(), "boolean_pointer_identity_matches_pool");

    auto tChar = global.searchEntry<BasicType>("char", &localZone);
    ok &= expect(tChar == pool.character(), "char_pointer_identity_matches_pool");

    ok &= expect(pool.findByName("integer") == pool.integer(), "pool_find_by_name_integer");
    ok &= expect(pool.findByName("missing") == nullptr, "pool_find_by_name_missing");

    ok &= expect(!pool.installInto(nullptr), "install_into_null_table_set_fails");

    return ok ? 0 : 1;
}
