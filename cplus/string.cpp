/* ==== Macros ==== */

#ifdef CPLUS_NAMESPACE

#define string(s)            String {(CPLUS_NAMESPACE::u8*) s, sizeof(s) - 1}
#define utf8_string(s)       String {(CPLUS_NAMESPACE::u8*) u8 ## s, sizeof(u8 ## s) - 1}
#define utf16_string(s)      String {(CPLUS_NAMESPACE::u8*) u ## s, sizeof(u ## s) - sizeof(u16)}
#define utf32_string(s)      String {(CPLUS_NAMESPACE::u8*) U ## s, sizeof(U ## s) - sizeof(u32)}

#define array_string(s)      String {(CPLUS_NAMESPACE::u8*) s, sizeof(s)}
#define data_string(s)       String {(CPLUS_NAMESPACE::u8*) &s, sizeof(s)}

#else

#define string(s)            String {(u8*) s, sizeof(s) - 1}
#define utf8_string(s)       String {(u8*) u8 ## s, sizeof(u8 ## s) - 1}
#define utf16_string(s)      String {(u8*) u ## s, sizeof(u ## s) - sizeof(u16)}
#define utf32_string(s)      String {(u8*) U ## s, sizeof(U ## s) - sizeof(u32)}

#define array_string(s)      String {(u8*) s, sizeof(s)}
#define data_string(s)       String {(u8*) &s, sizeof(s)}

#endif

#define quoted_string(s)     string(#s)




/* ==== String ==== */

struct String {

    u8* data;
    u64 count;


    /* ---- Core ---- */

    inline u8 operator [] (u64 index) {
        return data[index];
    }

    inline String view(u64 start, u64 end) {
        return { data + start, end - start };
    }

    inline String advance(u64 pos) {
        return { data + pos, count - pos };
    }

    inline Tuple3<String, String, bool> split(u64 pos) {
        if (pos > count) return {};
        return { view(0, pos), view(pos, count), true };
    }

    inline Tuple3<String, String, bool> split_from_end(u64 pos) {
        if (pos > count) return {};
        return split(count - pos);
    }



    /* ---- Basic Utils ---- */

    // sigh...
    static constexpr u8 spaces_lookup_table[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    };

    inline void build_lookup_table(u8 table[256]) {
        for (u64 i = 0; i < count; i++) {
            table[data[i]] = 1;
        }
    }



    /* ---- Match ---- */

    bool contains(u8 c) {
        for (u64 i = 0; i < count; i++) {
            if (data[i] == c) return true;
        }
        return false;
    }

    bool contains(String s) {
        if (count < s.count) return false;
        for (u64 i = 0; i < count - s.count + 1; i++) {
            for (u64 j = 0; j < s.count; j++) {
                if (data[i + j] != s[j]) goto next;
            }
            return true;
            next: continue;
        }
        return false;
    }

    bool contains_any(const u8 table[256]) {
        for (u64 i = 0; i < count; i++) {
            if (table[data[i]]) return true;
        }
        return false;
    }

    bool contains_any(String s) {
        u8 table[256];
        s.build_lookup_table(table);
        return contains_any(table);
    }

    bool starts_with(u8 c) {
        if (!count) return false;
        return data[0] == c;
    }

    bool ends_with(u8 c) {
        if (!count) return false;
        return data[count - 1] == c;
    }

    bool starts_with(String match) {
        if (count < match.count) return false;
        for (u64 i = 0; i < match.count; i++) {
            if (data[i] != match[i]) return false;
        }
        return true;
    }

    bool ends_with(String match) {
        if (count < match.count) return false;
        u64 pos = count - match.count;
        for (u64 i = 0; i < match.count; i++) {
            if (data[pos + i] != match[i]) return false;
        }
        return true;
    }

    bool starts_with_any(String match) {

        if (!count) return false;

        u8 c = data[0];
        for (u64 i = 0; i < match.count; i++) {
            if (match[i] == c) return true;
        }

        return false;
    }

    bool ends_with_any(String match) {

        if (!count) return false;

        u8 c = data[count - 1];
        for (u64 i = 0; i < match.count; i++) {
            if (match[i] == c) return true;
        }

        return false;
    }

    Tuple2<String, bool> find(u8 c) {
        for (u64 i = 0; i < count; i++) {
            if (data[i] == c) return { { data + i, count - i }, true };
        }
        return {};
    }

    Tuple2<String, bool> find(String s) {
        if (!count || !s.count || !data || !s.data || (s.count > count)) return {};
        for (u64 i = 0; i < count - s.count + 1; i++) {
            for (u64 j = 0; j < s.count; j++) {
                if (data[i + j] != s[j]) goto next;
            }
            return { { data + i, count - i }, true };
            next: continue;
        }
        return {};
    }





    /* ---- Trim & Eat Utils ---- */

    u64 eat_until(u8 c) {

        u64 out = 0;
        for (u64 i = 0; i < count; i++) {
            if (data[i] != c) out++;
            else              break;
        }

        data  += out;
        count -= out;

        return out;
    }

    u64 eat_matches(const u8 table[256]) {

        u64 out = 0;
        for (u64 i = 0; i < count; i++) {
            if (table[data[i]]) out++;
            else                break;
        }

        data  += out;
        count -= out;

        return out;
    }

    u64 eat_not_matches(const u8 table[256]) {

        u64 out = 0;
        for (u64 i = 0; i < count; i++) {
            if (!table[data[i]]) out++;
            else                 break;
        }

        data  += out;
        count -= out;

        return out;
    }

    u64 eat_matches(String s) {
        u8 table[256] = {};
        s.build_lookup_table(table);
        return eat_matches(table);
    }

    u64 eat_not_matches(String s) {
        u8 table[256] = {};
        s.build_lookup_table(table);
        return eat_not_matches(table);
    }



    /* ---- Trimming ---- */

    String trim_any_from_start(const u8 table[256]) {
        auto s = *this;
        s.eat_matches(table);
        return s;
    }

    String trim_any_from_end(const u8 table[256]) {
        for (u64 i = count; i > 0;) {
            i--;
            if (!table[data[i]]) return { data, i + 1 };
        }
        return {};
    }

    String trim_any(const u8 table[256]) {
        auto s = *this;
        s.eat_matches(table);
        return s.trim_any_from_end(table);
    }

    String trim_any_from_start(String match) {
        u8 table[256] = {};
        auto s = *this;
        match.build_lookup_table(table);
        s.eat_matches(table);
        return s;
    }

    String trim_any_from_end(String match) {
        u8 table[256] = {};
        match.build_lookup_table(table);
        return trim_any_from_end(table);
    }

    String trim_any(String match) {
        u8 table[256] = {};
        match.build_lookup_table(table);
        return trim_any(table);
    }

    String trim_spaces_from_start() {
        return trim_any_from_start(spaces_lookup_table);
    }

    String trim_spaces_from_end() {
        return trim_any_from_end(spaces_lookup_table);
    }

    String trim_spaces() {
        return trim_any(spaces_lookup_table);
    }

    String trim_prefix(String prefix) {
        u8 ok = starts_with(prefix);
        if (!ok) return *this;
        return advance(prefix.count);
    }

    String trim_suffix(String suffix) {
        u8 ok = ends_with(suffix);
        if (!ok) return *this;
        return view(0, count - suffix.count);
    }


    /* ---- Lazy Splitting ---- */

    String eat_by_separator(u8 c) {

        u8* out_data  = data;
        u64 out_count = eat_until(c);

        if (count) {
            data++;
            count--;
        }

        if (!out_count) return {};

        return { out_data, out_count };
    }

    String eat_by_any_matches(const u8 table[256]) {

        eat_not_matches(table);

        u8* out_data  = data;
        u64 out_count = eat_matches(table);

        eat_not_matches(table);

        return { out_data, out_count };
    }

    String eat_by_any_separators(const u8 table[256]) {

        eat_matches(table);

        u8* out_data  = data;
        u64 out_count = eat_not_matches(table);

        eat_matches(table);

        return { out_data, out_count };
    }

    // note: this is slower than table version, but easier to use
    String eat_by_any_matches(String match) {
        u8 table[256] = {};
        match.build_lookup_table(table);
        return eat_by_any_matches(table);
    }

    // note: this is slower than table version, but easier to use
    String eat_by_any_separators(String separators) {
        u8 table[256] = {};
        separators.build_lookup_table(table);
        return eat_by_any_separators(table);
    }

    String eat_by_spaces() {
        return eat_by_any_separators(spaces_lookup_table);
    }

    String eat_line() {
        auto line = eat_by_separator('\n');
        if (!line.count) return line;
        if (line.data[line.count - 1] == '\r') line.count--;
        return line;
    }

    String eat_by_separator(String separator) {

        auto [found, ok] = find(separator);
        if (!ok) {
            auto out = *this;
            *this = {};
            return out;
        }

        String out = { data, (u64) (found.data) - (u64) data };
        *this = advance(out.count + separator.count);

        return out;
    }

    String eat_by_separator_excluding_empty(String separator) {

        u8* out_data  = NULL;
        u64 out_count = 0;

        while (!out_count) {

            auto [found, ok] = find(separator);
            if (!ok) {
                auto out = *this;
                *this = {};
                return out;
            }

            out_data  = data;
            out_count = found.data - data;

            *this = advance(out_count + separator.count);
        }

        return { out_data, out_count };
    }




    /* ---- Allocating ---- */

    static Tuple2<String, bool> alloc(u64 count, Allocator allocator = Allocators::default_heap) {
        u8* data = (u8*) allocator.alloc_and_clear(count * sizeof(u8));
        if (!data) return {};
        return { { data, count }, true };
    }

    Tuple2<String, bool> copy(Allocator allocator = Allocators::default_heap) {
        auto [out, ok] = alloc(count, allocator);
        if (!ok) return {};
        memcpy(out.data, data, count);
        return { out, ok };
    }

    static Tuple2<String, bool> concat(Array<String> in, Allocator allocator = Allocators::default_heap) {

        if (in.count == 0) return {};
        if (in.count <  2) return { in[0], true };

        u64 count = 0;
        for (u64 i = 0; i < in.count; i++)  count += in[i].count;

        auto [out, ok] = alloc(count, allocator);
        if (!ok) return {};

        u64 acc = 0;
        for (u64 i = 0; i < in.count; i++) {
            append_buffer(out.data, &acc, in[i]);
        }

        assert(acc == count);

        return { out, ok };
    }

    static Tuple2<String, bool> join(Array<String> in, String separator = {}, bool end_with_separator = false, Allocator allocator = Allocators::default_heap) {

        if (in.count == 0) return {};
        if (in.count <  2) return { in[0], true };

        u64 count = 0;
        for (u64 i = 0; i < in.count; i++) count += in[i].count;
        count += separator.count * (end_with_separator ? in.count : (in.count - 1));

        auto [out, ok] = alloc(count, allocator);
        if (!ok) return {};

        u64 acc = 0;
        for (u64 i = 0; i < in.count - 1; i++) {
            append_buffer(out.data, &acc, in[i]);
            append_buffer(out.data, &acc, separator);
        }

        append_buffer(out.data, &acc, in[in.count - 1]);
        if (end_with_separator) append_buffer(out.data, &acc, separator);

        assert(acc == count);

        return { out, ok };
    }

    Tuple2<String, bool> replace(String a, String b, Allocator allocator = Allocators::default_heap) {

        u64 to_alloc = 0;
        for (auto walk = *this; walk.count;) {

            auto chunk = walk;
            auto [found, ok] = walk.find(a);

            if (ok) chunk = { walk.data, (u64) found.data - (u64) walk.data };
            to_alloc += chunk.count;

            if (!ok) break;
            to_alloc += b.count;

            walk = walk.advance(chunk.count + a.count);
        }

        auto [out, ok] = alloc(to_alloc, allocator);
        if (!ok) return {};

        u64 acc = 0;
        for (auto walk = *this; walk.count;) {

            auto chunk = walk;
            auto [found, find_ok] = walk.find(a);

            if (find_ok) chunk = { walk.data, (u64) found.data - (u64) walk.data };
            append_buffer(out.data, &acc, chunk);

            if (!find_ok) break;
            append_buffer(out.data, &acc, b);

            walk = walk.advance(chunk.count + a.count);
        }

        assert(acc == to_alloc);

        return { out, ok };
    }


    /* ---- Utils ---- */

    static String from_c_string(char* s) {
        return { (u8*) s, (u64) strlen(s) };
    }

    char* to_c_string(Allocator allocator = Allocators::default_heap) {

        auto p = (char*) allocator.alloc(count + 1);
        if (!p) return NULL;

        p[count] = '\0';
        memcpy(p, data, count);

        return p;
    }

    inline Array<u8> to_array() {
        return { data, count };
    }

    static s32 compare(String a, String b) {

        auto min = Math::min(a.count, b.count);

        for (u64 i = 0; i < min; i++) {
            auto x = a[i];
            auto y = b[i];
            if (x < y) return -1;
            if (x > y) return  1;
        }

        return 0;
    }

private:
    // note: unsafe
    static inline void append_buffer(u8* data, u64* acc, String s) {
        for (u64 i = 0; i < s.count; i++) data[*acc + i] = s.data[i];
        *acc += s.count;
    }
};

bool operator == (String a, String b) {
    if (a.count != b.count) return false;
    if (a.data  == b.data ) return true;
    for (u64 i = 0; i < a.count; i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool operator != (String a, String b) {
    return !(a == b);
}

bool memory_equal(String a, String b) {
    return a.count == b.count && a.data == b.data;
}




/* ==== StringBuilder ==== */

struct StringBuilder : String {

    u64       allocated;
    Allocator allocator;

    bool init(u64 init_count = 1024, Allocator init_alloc = Allocators::default_heap) {

        assert(init_count != 0);

        *this = {};

        allocator = init_alloc;

        auto p = (u8*) allocator.alloc(init_count);
        if (!p) return false;

        this->data  = p;
        this->count = 0;
        allocated   = init_count;

        return true;
    }

    bool init_from(String in, Allocator init_alloc = Allocators::default_heap) {

        if (!in.count) return init(1024, init_alloc);

        auto ok = init(round_to_next_power_of_2(in.count), init_alloc); // because the reason you make it dynamic is to append stuff
        if (!ok) return false;

        memcpy(this->data, in.data, in.count);
        this->count = in.count;

        return true;
    }

    void deinit() {
        allocator.free(this->data);
        *this = {};
    }

    void reset() {
        this->count = 0;
    }

    bool reserve(u64 new_size) {

        if (new_size <= allocated) return false;

        auto p = (u8*) allocator.resize(this->data, new_size);
        if (!p) return false;

        this->data = p;
        allocated  = new_size;

        return true;
    }

    u64 append(String s) {

        u64 wanted = this->count + s.count;
        if (wanted > allocated) {
            auto ok = reserve(round_to_next_power_of_2(wanted));
            if (!ok) return 0;
        }

        memcpy(this->data + this->count, s.data, s.count);
        this->count = wanted;

        return s.count;
    }

    template<typename T>
    u64 append(T c) {
        return append(data_string(c));
    }

    u64 print(String s, ...) {

        u64 old_count = count;

        va_list args;
        va_start(args, s);

        for (u64 i = 0; i < s.count; i++) {

            u8 c = s[i];
            if (c == '@') {
                if (i + 1 < s.count && s[i + 1] == '@') { // short circuit
                    append('@');
                    i++;
                } else {
                    append(va_arg(args, String)); // not safe, but this is C varargs, what can you do
                }
                continue;
            }

            append(c);
        }

        va_end(args);

        return count - old_count;
    }

    u64 print(const char* s, ...) {

        va_list va, va2;
        va_start(va, s);
        va_copy(va2, va);

        u64 format_count = 0; // note: does not include the null terminator!
        {
            int c = vsnprintf(0, 0, s, va);
            if (c <= 0) return 0;
            format_count = (u64) c;
        }

        u64 wanted = this->count + format_count;
        while (wanted + 1 > allocated) {
            auto ok = reserve(allocated * 2);
            if (!ok) return 0;
        }

        vsnprintf((char*) this->data + this->count, format_count + 1, s, va2);
        this->count = wanted;

        va_end(va);
        va_end(va2);

        return format_count;
    }
};




/* ==== Inplace String ==== */

template<u64 size>
struct InplaceString {

    u8 data[size - 1];
    u8 count;

    inline u8 operator [] (u64 index) {
        return data[index];
    }

    static InplaceString<size> from_string(String s) {

        assert(s.count < size);

        InplaceString<size> out;
        out.count = (u8) s.count;

        memcpy(out.data, s.data, s.count);

        return out;
    }

    inline String to_string() {
        return { data, count };
    }

    template<u64 new_size>
    inline InplaceString<new_size> resize() {

        if constexpr (new_size < size) assert(((u64) count + 1) < new_size);

        auto out = *(InplaceString<new_size>*) this;
        out.count = count;

        return out;
    }

    static_assert((size >= 4 && size <= 256) && (size % 2 == 0));
};

