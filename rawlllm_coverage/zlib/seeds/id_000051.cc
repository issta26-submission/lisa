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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers (allowed here so the function is self-contained for compilation).
    // Note: In normal projects, put includes at file top-level.
    #include <zlib.h>
    #include <vector>
    #include <cstring>

    // Original data to compress/decompress
    const char *original = "The quick brown fox jumps over the lazy dog";
    const uLong original_len = static_cast<uLong>(std::strlen(original)); // do not include trailing NUL

    // 1) Compute checksums (adler32 and crc32)
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(original_len));
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(original), static_cast<uInt>(original_len));

    // 2) Compress with simple one-shot API compress2()
    uLong bound = compressBound(original_len);
    std::vector<Bytef> comp_buf(bound);
    uLongf comp_len = static_cast<uLongf>(bound);
    int r = compress2(comp_buf.data(), &comp_len,
                      reinterpret_cast<const Bytef*>(original), original_len,
                      Z_BEST_COMPRESSION);
    if (r != Z_OK) {
        // compression failed
        return -1;
    }

    // 3) Uncompress with one-shot API uncompress()
    std::vector<Bytef> decomp_buf(original_len);
    uLongf decomp_len = original_len;
    r = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len);
    if (r != Z_OK) {
        return -2;
    }
    if (decomp_len != original_len) {
        return -3;
    }
    if (std::memcmp(decomp_buf.data(), original, original_len) != 0) {
        return -4;
    }

    // 4) Streamed deflate (inflate/deflate) example: compress original in streaming way
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    r = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (r != Z_OK) {
        return -5;
    }
    // Provide input in one chunk for simplicity
    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(original));
    defstream.avail_in = static_cast<uInt>(original_len);

    // Output buffer for streaming
    std::vector<Bytef> scomp_buf(compressBound(original_len));
    defstream.next_out = scomp_buf.data();
    defstream.avail_out = static_cast<uInt>(scomp_buf.size());

    r = deflate(&defstream, Z_FINISH);
    if (!(r == Z_STREAM_END || r == Z_OK)) {
        deflateEnd(&defstream);
        return -6;
    }
    // Number of bytes produced:
    size_t scomp_len = scomp_buf.size() - defstream.avail_out;
    // finalize
    deflateEnd(&defstream);

    // 5) Streamed inflate to recover data from scomp_buf
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    r = inflateInit(&infstream);
    if (r != Z_OK) {
        return -7;
    }
    infstream.next_in = scomp_buf.data();
    infstream.avail_in = static_cast<uInt>(scomp_len);

    std::vector<Bytef> sdecomp_buf(original_len);
    infstream.next_out = sdecomp_buf.data();
    infstream.avail_out = static_cast<uInt>(sdecomp_buf.size());

    r = inflate(&infstream, Z_FINISH);
    if (r != Z_STREAM_END && r != Z_OK) {
        inflateEnd(&infstream);
        return -8;
    }
    size_t sdecomp_len = sdecomp_buf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    if (sdecomp_len != original_len) {
        return -9;
    }
    if (std::memcmp(sdecomp_buf.data(), original, original_len) != 0) {
        return -10;
    }

    // 6) Validate that checksums computed earlier match recomputed values on decompressed data
    uLong adler_check = adler32(0L, Z_NULL, 0);
    adler_check = adler32(adler_check, sdecomp_buf.data(), static_cast<uInt>(sdecomp_len));
    uLong crc_check = crc32(0L, Z_NULL, 0);
    crc_check = crc32(crc_check, sdecomp_buf.data(), static_cast<uInt>(sdecomp_len));
    if (adler_check != adler || crc_check != crc) {
        return -11;
    }

    // If all went fine, return the requested success code 66
    return 66;
}