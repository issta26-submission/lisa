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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare sample input data by repeating a base sentence.
    std::string base = "The quick brown fox jumps over the lazy dog. ";
    std::string input_str;
    for (int i = 0; i < 100; ++i) input_str += base; // about 4400+ bytes

    // Convert to a mutable byte buffer for zlib streaming APIs.
    std::vector<Bytef> input_buf(input_str.begin(), input_str.end());
    uLong sourceLen = static_cast<uLong>(input_buf.size());

    // 1) Use compressBound and compress2 to compress the whole buffer.
    uLong bound = compressBound(sourceLen);
    std::vector<Bytef> comp_buf(bound);
    uLong comp_len = bound;
    int zret = compress2(comp_buf.data(), &comp_len, input_buf.data(), sourceLen, Z_BEST_COMPRESSION);
    if (zret != Z_OK) return 1;

    // 2) Use uncompress to recover the original data and validate.
    std::vector<Bytef> uncomp_buf(sourceLen);
    uLong uncomp_len = sourceLen;
    zret = uncompress(uncomp_buf.data(), &uncomp_len, comp_buf.data(), comp_len);
    if (zret != Z_OK) return 2;
    if (uncomp_len != sourceLen) return 3;
    if (std::memcmp(uncomp_buf.data(), input_buf.data(), sourceLen) != 0) return 4;

    // 3) Compute and compare checksums (crc32 and adler32) on original and uncompressed data.
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, input_buf.data(), static_cast<uInt>(sourceLen));
    uLong crc_uncomp = crc32(0L, Z_NULL, 0);
    crc_uncomp = crc32(crc_uncomp, uncomp_buf.data(), static_cast<uInt>(uncomp_len));
    if (crc_orig != crc_uncomp) return 5;

    uLong adl_orig = adler32(0L, Z_NULL, 0);
    adl_orig = adler32(adl_orig, input_buf.data(), static_cast<uInt>(sourceLen));
    uLong adl_uncomp = adler32(0L, Z_NULL, 0);
    adl_uncomp = adler32(adl_uncomp, uncomp_buf.data(), static_cast<uInt>(uncomp_len));
    if (adl_orig != adl_uncomp) return 6;

    // 4) Demonstrate streaming deflate (z_stream + deflateInit / deflate / deflateEnd).
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    zret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (zret != Z_OK) return 7;

    const size_t CHUNK = 16384;
    std::vector<Bytef> deflated_stream;
    defstrm.next_in = input_buf.data();
    defstrm.avail_in = static_cast<uInt>(sourceLen);

    // Repeatedly call deflate with Z_FINISH until stream ends.
    for (;;) {
        std::vector<Bytef> out(CHUNK);
        defstrm.next_out = out.data();
        defstrm.avail_out = static_cast<uInt>(CHUNK);

        zret = deflate(&defstrm, Z_FINISH);
        if (zret != Z_OK && zret != Z_STREAM_END) {
            deflateEnd(&defstrm);
            return 8;
        }
        size_t have = CHUNK - defstrm.avail_out;
        deflated_stream.insert(deflated_stream.end(), out.begin(), out.begin() + have);

        if (zret == Z_STREAM_END) break;
    }

    deflateEnd(&defstrm);

    // 5) Demonstrate streaming inflate (z_stream + inflateInit / inflate / inflateEnd).
    z_stream instrm;
    std::memset(&instrm, 0, sizeof(instrm));
    zret = inflateInit(&instrm);
    if (zret != Z_OK) return 9;

    instrm.next_in = deflated_stream.data();
    instrm.avail_in = static_cast<uInt>(deflated_stream.size());

    std::vector<Bytef> inflated_stream;
    for (;;) {
        std::vector<Bytef> out(CHUNK);
        instrm.next_out = out.data();
        instrm.avail_out = static_cast<uInt>(CHUNK);

        zret = inflate(&instrm, Z_NO_FLUSH);
        if (zret != Z_OK && zret != Z_STREAM_END) {
            inflateEnd(&instrm);
            return 10;
        }
        size_t have = CHUNK - instrm.avail_out;
        inflated_stream.insert(inflated_stream.end(), out.begin(), out.begin() + have);

        if (zret == Z_STREAM_END) break;
    }

    inflateEnd(&instrm);

    if (inflated_stream.size() != sourceLen) return 11;
    if (std::memcmp(inflated_stream.data(), input_buf.data(), sourceLen) != 0) return 12;

    // 6) Demonstrate the simple compress/uncompress pair as well (compress/uncompress already used,
    //    but here we show compress() API specifically).
    uLong comp_simple_bound = compressBound(sourceLen);
    std::vector<Bytef> comp_simple(comp_simple_bound);
    uLong comp_simple_len = comp_simple_bound;
    zret = compress(comp_simple.data(), &comp_simple_len, input_buf.data(), sourceLen);
    if (zret != Z_OK) return 13;

    std::vector<Bytef> uncomp_simple(sourceLen);
    uLong uncomp_simple_len = sourceLen;
    zret = uncompress(uncomp_simple.data(), &uncomp_simple_len, comp_simple.data(), comp_simple_len);
    if (zret != Z_OK) return 14;
    if (uncomp_simple_len != sourceLen) return 15;
    if (std::memcmp(uncomp_simple.data(), input_buf.data(), sourceLen) != 0) return 16;

    // All zlib API sequences succeeded.
    return 66;
}