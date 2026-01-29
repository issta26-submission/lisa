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
//<ID> 95
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers (these preprocessor directives are valid inside a function).
    // If you already include <zlib.h> and other headers in your translation unit,
    // you can remove these lines.
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>

    // Example input data
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a sample string to demonstrate zlib API usage. "
                              "Repeat: The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog.";

    const uLong src_len = static_cast<uLong>(input.size());

    // 1) Compute CRC32 of the input
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before, reinterpret_cast<const Bytef*>(input.data()), src_len);

    // 2) Use the simple compress()/uncompress() convenience API
    uLong bound = compressBound(src_len);
    std::vector<Bytef> comp1(bound);
    uLong comp1_len = bound;

    int ret = compress(comp1.data(), &comp1_len,
                       reinterpret_cast<const Bytef*>(input.data()), src_len);
    if (ret != Z_OK) return 1; // compress failed

    // Decompress with uncompress() and verify equality
    std::vector<Bytef> decomp1(src_len);
    uLong decomp1_len = src_len;
    ret = uncompress(decomp1.data(), &decomp1_len, comp1.data(), comp1_len);
    if (ret != Z_OK) return 2; // uncompress failed
    if (decomp1_len != src_len) return 3;
    if (std::memcmp(decomp1.data(), input.data(), src_len) != 0) return 4;

    // 3) Use the streaming deflate API (deflateInit / deflate / deflateEnd)
    z_stream strm_compress;
    std::memset(&strm_compress, 0, sizeof(strm_compress));
    ret = deflateInit(&strm_compress, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    // Prepare output buffer for streaming deflate (use compressBound to be safe)
    std::vector<Bytef> comp2(bound);
    strm_compress.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    strm_compress.avail_in = src_len;
    strm_compress.next_out = comp2.data();
    strm_compress.avail_out = static_cast<uInt>(comp2.size());

    // Call deflate with Z_FINISH since we provide all data at once
    ret = deflate(&strm_compress, Z_FINISH);
    if (!(ret == Z_STREAM_END || ret == Z_OK)) {
        deflateEnd(&strm_compress);
        return 6;
    }

    // Calculate compressed size
    uLong comp2_len = comp2.size() - strm_compress.avail_out;
    ret = deflateEnd(&strm_compress);
    if (ret != Z_OK) return 7;

    // 4) Use the streaming inflate API (inflateInit / inflate / inflateEnd)
    z_stream strm_inflate;
    std::memset(&strm_inflate, 0, sizeof(strm_inflate));
    ret = inflateInit(&strm_inflate);
    if (ret != Z_OK) return 8;

    std::vector<Bytef> decomp2(src_len);
    strm_inflate.next_in = comp2.data();
    strm_inflate.avail_in = static_cast<uInt>(comp2_len);
    strm_inflate.next_out = decomp2.data();
    strm_inflate.avail_out = static_cast<uInt>(decomp2.size());

    ret = inflate(&strm_inflate, Z_FINISH);
    if (!(ret == Z_STREAM_END || ret == Z_OK)) {
        inflateEnd(&strm_inflate);
        return 9;
    }

    uLong decomp2_len = decomp2.size() - strm_inflate.avail_out;
    ret = inflateEnd(&strm_inflate);
    if (ret != Z_OK) return 10;

    if (decomp2_len != src_len) return 11;
    if (std::memcmp(decomp2.data(), input.data(), src_len) != 0) return 12;

    // 5) Compute CRC32 after round-trip and verify it matches
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, decomp2.data(), decomp2_len);
    if (crc_before != crc_after) return 13;

    // If we've reached this point, all zlib API sequences used above worked correctly.
    return 66;
}