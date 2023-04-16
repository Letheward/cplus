/* ==== Print ==== */

void file_print_string(FILE* f, String s) {
    fwrite(s.data, sizeof(u8), s.count, f);
}

void print_string(String s) {
    fwrite(s.data, sizeof(u8), s.count, stdout);
}

// todo: not robust, need more testing, handle adjacent items (no space in between)
void print(String s, ...) {

    va_list args;
    va_start(args, s);

    for (u64 i = 0; i < s.count; i++) {

        u8 c = s[i];
        if (c == '@') {
            if (i + 1 < s.count && s[i + 1] == '@') { // short circuit
                putchar('@');
                i++;
            } else {
                print_string(va_arg(args, String)); // not safe, but this is C varargs, what can you do
            }
            continue;
        }

        putchar(c);
    }

    va_end(args);
}

// todo: solve this copy pasta
void file_print(FILE* f, String s, ...) {
    
    va_list args;
    va_start(args, s);
    
    for (u64 i = 0; i < s.count; i++) {

        u8 c = s[i];
        if (c == '@') {
            if (i + 1 < s.count && s[i + 1] == '@') { // short circuit 
                fputc('@', f);
                i++;
            } else {
                file_print_string(f, va_arg(args, String)); // not safe, but this is C varargs, what can you do 
            }
            continue;
        }

        fputc(c, f);
    }
    
    va_end(args);
}

void file_print_quoted_string(FILE* f, String s) {

    fputc('"', f);

    for (u64 i = 0; i < s.count; i++) {

        u8 c = s[i];
        switch (c) {

            case '"':  fprintf(f, "\\\""); break;
            case '\\': fprintf(f, "\\\\"); break;
            case '\b': fprintf(f, "\\b");  break;
            case '\f': fprintf(f, "\\f");  break;
            case '\n': fprintf(f, "\\n");  break;
            case '\r': fprintf(f, "\\r");  break;
            case '\t': fprintf(f, "\\t");  break;

            default:
            {
                if (c < 0x20) fprintf(f, "\\u%.4x", c);
                else          fputc(c, f);
                break;
            }
        }
    }

    fputc('"', f);
}

void print_quoted_string(String s) {
    file_print_quoted_string(stdout, s);
}




/* ==== Read From Console ==== */

// note: does not give a copy
String read_line(String buffer) {
    
    String s = buffer;
    fgets((char*) s.data, (int) s.count, stdin);
    s.count = strlen((const char*) s.data);
    
    return s.trim_any_from_end(string("\r\n"));
}






/* ==== File IO ==== */

Tuple2<String, bool> load_file(const char* path, Allocator allocator = Allocators::default_heap) {

    FILE* f = fopen(path, "rb");
    if (!f) return {};

    fseek(f, 0, SEEK_END);
    u64 count = ftell(f);
    fseek(f, 0, SEEK_SET);

    u8* data = (u8*) allocator.alloc(count);
    if (!data) {
        fclose(f);
        return {};
    }

    fread(data, 1, count, f);
    fclose(f);

    return { {data, count}, true };
}

bool save_file(String in, const char* path) {

    FILE* f = fopen(path, "wb");
    if (!f) return false;

    fwrite(in.data, sizeof(u8), in.count, f);
    fflush(f);
    fclose(f);

    return true;
}

