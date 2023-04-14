/* ==== Unicode ==== */

Tuple2<u32, u64> get_codepoint_from_utf8(String s) {

    u8* p     = s.data;
    u64 count = s.count;
    if (count < 1) goto fail;

    if ((p[0] & 0xf8) == 0xf0) {
        if (count < 4) goto fail;
        return { (u32) ((p[0] & 0x07) << 18) | ((p[1] & 0x3f) << 12) | ((p[2] & 0x3f) << 6) | (p[3] & 0x3f), 4 };
    }

    if ((p[0] & 0xf0) == 0xe0) {
        if (count < 3) goto fail;
        return { (u32) ((p[0] & 0x0f) << 12) | ((p[1] & 0x3f) << 6) | (p[2] & 0x3f), 3 };
    }

    if ((p[0] & 0xe0) == 0xc0) {
        if (count < 2) goto fail;
        return { (u32) ((p[0] & 0x1f) <<  6) | (p[1] & 0x3f), 2 };
    }

    return { (u32) p[0], 1 };

    fail: return { 0, 0 };
}

Tuple2<u32, u64> get_codepoint_from_utf16(String s) {

    u16* p     = (u16*) s.data;
    u64  count = s.count;
    if (count < 1 * sizeof(u16)) goto fail;

    if (p[0] >= 0xd800 && p[0] <= 0xdfff) {
        if (count < 2 * sizeof(u16)) goto fail;
        return { (u32) (p[0] - 0xd800) * 0x400 + (p[1] - 0xdc00) + 0x10000, 2 * sizeof(u16) };
    }

    return { p[0], 1 * sizeof(u16) };

    fail: return { 0, 0 };
}

// note: this return byte count
// todo: validate
u64 codepoint_to_utf8(u32 codepoint, u8 out[4]) {

    if (codepoint <= 0x7f) {
        out[0] = (u8) codepoint;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        return 1;
    }

    if (codepoint <= 0x07ff) {
        out[0] = ((codepoint & 0x07c0) >> 6) | 0xc0;
        out[1] = ((codepoint & 0x003f))      | 0x80;
        out[2] = 0;
        out[3] = 0;
        return 2;
    }

    if (codepoint <= 0xffff) {
        out[0] = ((codepoint & 0xf000) >> 12) | 0xe0;
        out[1] = ((codepoint & 0x0fc0) >>  6) | 0x80;
        out[2] = ((codepoint & 0x003f))       | 0x80;
        out[3] = 0;
        return 3;
    }

    if (codepoint <= 0x10ffff) {
        out[0] = ((codepoint & 0x1c0000) >> 18) | 0xf0;
        out[1] = ((codepoint & 0x03f000) >> 12) | 0x80;
        out[2] = ((codepoint & 0x000fc0) >>  6) | 0x80;
        out[3] = ((codepoint & 0x00003f))       | 0x80;
        return 4;
    }

    // out of unicode range, we fail at this point
    {
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        return 0;
    }
}


// todo: handle fail case, like codepoint_to_utf8()
// note: this returns count of u16, not in bytes
// note: only handle little endian for now
u64 codepoint_to_utf16(u32 codepoint, u16 out[2]) {

    if (codepoint < 0x10000) {
        out[0] = (u16) codepoint;
        out[1] = 0;
        return 1;
    }

    u32 t  = codepoint - 0x10000;
    out[0] = (u16) ((t << 12) >> 22) + 0xd800;
    out[1] = (u16) ((t << 22) >> 22) + 0xdc00;

    return 2;
}

// note: only handle little endian for now
// note: null terminator is useful for Windows filenames, then you can pass output.data to CreateFileW(), etc.
// todo: validate
Tuple2<String, bool> utf8_to_utf16(String s, bool want_null_terminated = false, bool want_bom = false, Allocator allocator = Allocators::default_heap) {

    if (!s.count) return {};

    // first pass
    u64 to_alloc = 0;
    for (auto walk = s; walk.count;) {

        auto [point, to_advance] = get_codepoint_from_utf8(walk);
        if (!to_advance) break;

        u16 pair[2];
        to_alloc += sizeof(u16) * codepoint_to_utf16(point, pair); // todo: speed

        walk = walk.advance(to_advance);
    }

    if (want_null_terminated) to_alloc += sizeof(u16);
    if (want_bom)             to_alloc += sizeof(u16);
    auto [out, ok] = String::alloc(to_alloc, allocator);
    if (!ok) return {};

    // second pass
    {
        u16* out_u16 = (u16*) out.data;
        u64  acc     = 0; // note: this is count for u16 array, not for u8 array

        if (want_bom) {
            out_u16[0] = 0xfeff;
            acc++;
        }

        for (auto walk = s; walk.count;) {

            auto [point, to_advance] = get_codepoint_from_utf8(walk);
            if (!to_advance) break;

            u16 pair[2];
            u64 count = codepoint_to_utf16(point, pair);

            for (u64 i = 0; i < count; i++) {
                out_u16[acc] = pair[i];
                acc++;
            }
            
            walk = walk.advance(to_advance);
        }

        if (want_null_terminated) {
            out_u16[acc] = 0;
            acc++;
        }
        
        assert(acc * sizeof(u16) == to_alloc);
    }

    return { out, true };
}

// todo: validate
Tuple2<String, bool> utf16_to_utf8(String s, bool want_null_terminated = false, Allocator allocator = Allocators::default_heap) {

    if (!s.count) return {};

    // first pass
    u64 to_alloc = 0;
    for (auto walk = s; walk.count;) {

        auto [point, to_advance] = get_codepoint_from_utf16(walk);
        if (!to_advance) break;

        u8 bytes[4];
        to_alloc += codepoint_to_utf8(point, bytes); // todo: speed

        walk = walk.advance(to_advance);
    }

    if (want_null_terminated) to_alloc += 1;
    auto [out, ok] = String::alloc(to_alloc, allocator);
    if (!ok) return {};

    // second pass
    u64 acc = 0;
    for (auto walk = s; walk.count;) {

        auto [point, to_advance] = get_codepoint_from_utf16(walk);
        if (!to_advance) break;

        u8 bytes[4];
        u64 count = codepoint_to_utf8(point, bytes);

        for (u64 i = 0; i < count; i++) {
            out.data[acc] = bytes[i];
            acc++;
        }

        walk = walk.advance(to_advance);
    }

    if (want_null_terminated) {
        out.data[acc] = 0;
        acc++;
    }

    assert(acc == to_alloc);

    return { out, true };
}


