/* ==== Number Parsing ==== */

Tuple2<u64, bool> parse_u64(String s) {

    if (!s.count) return {};

    u64 result = 0;
    for (u64 i = 0; i < s.count; i++) {
        
        u8 c = s[i];
        if (c == '_') {
            if (i != 0) continue;
            else        return {};
        } else {
            if (c < '0' || c > '9') return {};
        }
        
        u64 next = result * 10 + (c - '0');
        if (next < result) return {}; // overflow
        
        result = next;
    }
    
    return { result, true };
}

// note: this includes the "0b" header
Tuple2<u64, bool> parse_binary_u64(String s) {

    if (s.count < 3) return {};
    if (s[0] != '0' || s[1] != 'b') return {};

    u64 result      = 0;
    u64 digit_count = 0;
    
    for (u64 i = 2; i < s.count; i++) {
        
        u8 c = s[i];
        if (c == '_') { 
            continue;
        } else {
            if (c < '0' || c > '1') return {};
        }
        
        result *= 2;
        result += (c == '1');
        
        digit_count++;
        if (digit_count > 64) return {};
    }

    return { result, true };
}

// note: this includes the "0x" header
Tuple2<u64, bool> parse_hex_u64(String s) {

    if (s.count < 3) return {};
    if (s[0] != '0' || s[1] != 'x') return {};

    u64 result      = 0;
    u64 digit_count = 0;
    
    for (u64 i = 2; i < s.count; i++) {
        
        u8 c = s[i];
        u64 digit = 0;

        if (c == '_') { 
            continue;
        } else {
            if (c >= '0' && c <= '9') {
                digit = c - '0';
            } else {
                if (c >= 'A' && c <= 'F') c += 'a' - 'A';
                if (c < 'a' || c > 'f') return {};
                digit = 10 + (c - 'a');
            }
        }
        
        result *= 16;
        result += digit;
        
        digit_count++;
        if (digit_count > 16) return {};
    }

    return { result, true };
}

Tuple2<s64, bool> parse_s64(String s) {

    if (!s.count) return {};

    s64 sign  = 1;
    u64 start = 0;
    switch (s[0]) {
        case '-': sign = -1; // fall-through
        case '+': start++;
        default:  break;
    }

    auto [result, ok] = parse_u64(s.advance(start));
    if (!ok) return {};
    
    s64 signed_result = (s64) result;
    if (signed_result < 0) return {}; // check overflow, overflow to 0 is handled by parse_u64()
    
    return { sign * signed_result, true };
}

// todo: validate
// todo: precision and handling overflow
Tuple2<f64, bool> parse_f64(String s) {
    
    if (!s.count) return {};

    u64 start = 0;
 
    f64 sign = 1;
    switch (s[start]) {
        case '-': sign = -1; // fall-through
        case '+': start++;
        default:  break;
    }
    
    u64 dot_pos = start;
    while (dot_pos < s.count) {
        if (s[dot_pos] == '.') break;
        dot_pos++;
    }
    
    f64 result = 0;
    for (u64 i = start; i < dot_pos; i++) {
        
        u8 c = s[i];
        if (c == '_') {
            if (i != start) continue;
            else            return {};
        } else if (c < '0' || c > '9') {
            return {};
        }
        
        result *= 10.0;
        result += c - '0';
    }

    f64 denom = 10;
    for (u64 i = dot_pos + 1; i < s.count; i++) {
        
        u8 c = s[i];
        if (c == '_')           continue;
        if (c < '0' || c > '9') return {};
        
        result += (f64) (c - '0') / denom;
        denom  *= 10;
    }
    
    return { sign * result, true };
}

