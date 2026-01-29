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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary headers for this function. Including inside the function keeps this
    // block self-contained; it's valid C++ to have preprocessor directives here.
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Sample input data
    const std::string input =
        "The quick brown fox jumps over the lazy dog. "
        "This is a sample string to exercise zlib compression and decompression APIs. "
        "Repeat the sentence a few times to make the buffer non-trivial. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. ";

    const uLong src_len = static_cast<uLong>(input.size());

    // 1) Compute checksums (crc32 and adler32) for original data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input.data()), src_len);

    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(input.data()), src_len);

    // 2) Use convenience functions compress2/uncompress
    uLong comp_bound = compressBound(src_len);
    std::vector<Bytef> comp_buf(comp_bound);
    uLong comp_len = comp_bound;

    int res = compress2(comp_buf.data(), &comp_len,
                        reinterpret_cast<const Bytef*>(input.data()), src_len,
                        Z_BEST_COMPRESSION);
    if (res != Z_OK) return 2; // compress2 failed

    std::vector<Bytef> decomp_buf(src_len);
    uLong decomp_len = src_len;
    res = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len);
    if (res != Z_OK) return 3; // uncompress failed
    if (decomp_len != src_len) return 4; // unexpected decompressed size
    if (std::memcmp(decomp_buf.data(), input.data(), src_len) != 0) return 5; // data mismatch

    // 3) Manual streaming deflate (deflateInit, deflate, deflateEnd)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    res = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) return 6;

    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    defstream.avail_in = src_len;

    std::vector<Bytef> stream_comp_buf(comp_bound);
    defstream.next_out = stream_comp_buf.data();
    defstream.avail_out = comp_bound;

    res = deflate(&defstream, Z_FINISH);
    if (res != Z_STREAM_END && res != Z_OK) {
        deflateEnd(&defstream);
        return 7;
    }
    uLong stream_comp_len = static_cast<uLong>(comp_bound - defstream.avail_out);

    // Properly end deflate
    if (deflateEnd(&defstream) != Z_OK) return 8;

    // 4) Manual streaming inflate (inflateInit, inflate, inflateEnd)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    res = inflateInit(&infstream);
    if (res != Z_OK) return 9;

    infstream.next_in = stream_comp_buf.data();
    infstream.avail_in = stream_comp_len;

    std::vector<Bytef> stream_decomp_buf(src_len);
    infstream.next_out = stream_decomp_buf.data();
    infstream.avail_out = src_len;

    res = inflate(&infstream, Z_FINISH);
    if (res != Z_STREAM_END && res != Z_OK) {
        inflateEnd(&infstream);
        return 10;
    }
    uLong stream_decomp_len = static_cast<uLong>(src_len - infstream.avail_out);

    if (inflateEnd(&infstream) != Z_OK) return 11;

    if (stream_decomp_len != src_len) return 12;
    if (std::memcmp(stream_decomp_buf.data(), input.data(), src_len) != 0) return 13;

    // 5) Verify checksums for the decompressed result (using values computed earlier)
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, stream_decomp_buf.data(), stream_decomp_len);
    if (crc_after != crc_orig) return 14;

    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, stream_decomp_buf.data(), stream_decomp_len);
    if (adler_after != adler_orig) return 15;

    // If everything passed, return 66 as requested
    return 66;
}