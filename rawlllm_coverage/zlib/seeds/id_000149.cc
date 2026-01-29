#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Source data to compress/decompress (include the terminating NUL to verify exact round-trip)
    const char* src = "The quick brown fox jumps over the lazy dog";
    size_t src_len = std::strlen(src) + 1; // include terminating NUL

    // Compute checksums on the original data
    uLong adler_before = adler32(0L, Z_NULL, 0);
    adler_before = adler32(adler_before, reinterpret_cast<const Bytef*>(src), static_cast<uInt>(src_len));
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before, reinterpret_cast<const Bytef*>(src), static_cast<uInt>(src_len));

    // --- Streaming deflate (compress) ---
    z_stream def;
    std::memset(&def, 0, sizeof(def));
    if (deflateInit(&def, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 1;
    }

    def.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    def.avail_in = static_cast<uInt>(src_len);

    std::vector<unsigned char> compressed;
    unsigned char temp_out[128];
    int ret;
    do {
        def.next_out = temp_out;
        def.avail_out = sizeof(temp_out);
        ret = deflate(&def, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&def);
            return 2;
        }
        size_t have = sizeof(temp_out) - def.avail_out;
        compressed.insert(compressed.end(), temp_out, temp_out + have);
    } while (ret != Z_STREAM_END);

    if (deflateEnd(&def) != Z_OK) {
        return 3;
    }

    // --- Streaming inflate (decompress) ---
    z_stream inf;
    std::memset(&inf, 0, sizeof(inf));
    if (inflateInit(&inf) != Z_OK) {
        return 4;
    }

    inf.next_in = compressed.data();
    inf.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decompressed;
    unsigned char temp_in[128];
    do {
        inf.next_out = temp_in;
        inf.avail_out = sizeof(temp_in);
        ret = inflate(&inf, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&inf);
            return 5;
        }
        size_t have = sizeof(temp_in) - inf.avail_out;
        decompressed.insert(decompressed.end(), temp_in, temp_in + have);
    } while (ret != Z_STREAM_END);

    if (inflateEnd(&inf) != Z_OK) {
        return 6;
    }

    // Verify decompressed size and contents match original
    if (decompressed.size() != src_len) {
        return 7;
    }
    if (std::memcmp(decompressed.data(), src, src_len) != 0) {
        return 8;
    }

    // Recompute checksums on decompressed data and compare
    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, decompressed.data(), static_cast<uInt>(decompressed.size()));
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, decompressed.data(), static_cast<uInt>(decompressed.size()));
    if (adler_after != adler_before) {
        return 9;
    }
    if (crc_after != crc_before) {
        return 10;
    }

    // --- Convenience API: compress2 / uncompress demonstration ---
    uLongf bound = compressBound(static_cast<uLong>(src_len));
    std::vector<unsigned char> comp2(bound);
    int zret = compress2(comp2.data(), &bound,
                         reinterpret_cast<const Bytef*>(src),
                         static_cast<uLong>(src_len),
                         Z_BEST_SPEED);
    if (zret != Z_OK) {
        return 11;
    }

    std::vector<unsigned char> uncompressed(src_len);
    uLongf un_len = static_cast<uLongf>(src_len);
    zret = uncompress(uncompressed.data(), &un_len, comp2.data(), bound);
    if (zret != Z_OK) {
        return 12;
    }
    if (un_len != src_len) {
        return 13;
    }
    if (std::memcmp(uncompressed.data(), src, src_len) != 0) {
        return 14;
    }

    // All zlib API sequences succeeded and results verified
    return 66;
}