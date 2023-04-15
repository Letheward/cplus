/* ==== Type Utils ==== */

void generate_type_category_function_code(const char** types, u64 count, const char* name) {

    const char* default_code  = "template<typename T> constexpr bool %s() { return false; };\n";
    const char* instance_code = "template<> constexpr bool %s<%s>() { return true; };\n";

    printf(default_code, name);
    
    for (u64 i = 0; i < count; i++) {
        printf(instance_code, name, types[i]);
    }
    printf("\n");
}

void generate_type_category_function_union(const char** functions, u64 count, const char* name) {

    if (count < 2) return;
    
    const char* code = 
        "template<typename T> constexpr bool %s() {\n"
        "    return "
    ;

    printf(code, name);

    for (u64 i = 0; i < count - 1; i++) {
        printf("%s<T>() || ", functions[i]);
    }
    printf("%s<T>();\n}\n\n", functions[count - 1]);
}

void generate_default_type_category_code() {
    
    const char* ui = "type_is_unsigned_integer";
    const char* si = "type_is_signed_integer";
    const char* f  = "type_is_float";
    const char* i  = "type_is_integer";
    const char* v  = "type_is_basic_value";
    
    #define get(s)   s, (sizeof(s) / sizeof(s[0]))

    {
        const char* types[] = { "u8", "u16", "u32", "u64" };
        generate_type_category_function_code(get(types), ui);
    }
    
    {
        const char* types[] = { "s8", "s16", "s32", "s64" };
        generate_type_category_function_code(get(types), si);
    }

    {
        const char* types[] = { "f32", "f64" };
        generate_type_category_function_code(get(types), f);
    }

    {
        const char* funcs[] = { ui, si };
        generate_type_category_function_union(get(funcs), i);
    }
    
    {
        const char* funcs[] = { i, f };
        generate_type_category_function_union(get(funcs), v);
    }

    #undef get
}




/* ==== Tuples ==== */

void generate_tuple_code(u64 count = 6) {

    for (u64 i = 2; i <= count; i++) {
        printf("template<");
        for (u64 j = 0; j < i - 1; j++) {
            printf("typename T%llu, ", j);
        }
        printf("typename T%llu>\n", i - 1);
        printf("struct Tuple%llu { ", i);
        for (u64 j = 0; j < i; j++) {
            printf("T%llu v%llu; ", j, j);
        }
        printf("};\n\n");
    }
}


