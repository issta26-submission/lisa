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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // sample input
    const char input[] = "The quick brown fox jumps over the lazy dog. Zlib API sequence test.";
    const size_t input_len = sizeof(input) - 1; // exclude terminating NUL

    // compute checksums for the original data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(input), static_cast<uInt>(input_len));

    // --- deflate (stream API) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 1;

    // allocate output buffer using compressBound
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp_buf(bound);
    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = comp_buf.data();
    defstream.avail_out = static_cast<uInt>(comp_buf.size());

    // Because output buffer is sized by compressBound, one call should suffice.
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 2;
    }
    uLong comp_size = defstream.total_out;
    deflateEnd(&defstream);

    // --- inflate (stream API) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) return 3;

    std::vector<Bytef> decomp_buf(input_len);
    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(decomp_buf.size());

    // As above, one inflate with enough output buffer should finish.
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 4;
    }
    uLong decomp_size = infstream.total_out;
    inflateEnd(&infstream);

    // verify size and contents
    if (decomp_size != input_len) return 5;
    if (std::memcmp(decomp_buf.data(), input, input_len) != 0) return 6;

    // --- compress() / uncompress() simple API (non-stream) ---
    uLongf comp2_len = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp2_buf(comp2_len);
    ret = compress(comp2_buf.data(), &comp2_len,
                   reinterpret_cast<const Bytef*>(input),
                   static_cast<uLong>(input_len));
    if (ret != Z_OK) return 7;

    uLongf decomp2_len = static_cast<uLongf>(input_len);
    std::vector<Bytef> decomp2_buf(decomp2_len);
    ret = uncompress(decomp2_buf.data(), &decomp2_len,
                     comp2_buf.data(), comp2_len);
    if (ret != Z_OK) return 8;
    if (decomp2_len != input_len) return 9;
    if (std::memcmp(decomp2_buf.data(), input, input_len) != 0) return 10;

    // --- verify checksums match original ---
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, decomp_buf.data(), static_cast<uInt>(decomp_size));
    if (crc_after != crc_orig) return 11;

    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, decomp_buf.data(), static_cast<uInt>(decomp_size));
    if (adler_after != adler_orig) return 12;

    // use zlibVersion() to demonstrate API usage (not required to check)
    const char* ver = zlibVersion();
    (void)ver; // silence unused variable warning

    // All checks passed
    return 66;
}