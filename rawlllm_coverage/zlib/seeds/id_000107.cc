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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data
    std::string original = "The quick brown fox jumps over the lazy dog. ";
    // repeat to have a bit more data
    for (int i = 0; i < 8; ++i) original += original;

    uLong srcLen = static_cast<uLong>(original.size());

    // 1) Compute CRC32 of original
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()), srcLen);

    // 2) Use compress() / uncompress()
    uLong bound = compressBound(srcLen);
    std::vector<Bytef> compressed1(bound);
    uLongf comp1Len = bound;
    int ret = compress(compressed1.data(), &comp1Len,
                       reinterpret_cast<const Bytef*>(original.data()), srcLen);
    if (ret != Z_OK) return -1;

    // shrink vector to actual size
    compressed1.resize(comp1Len);

    // uncompress and verify
    std::vector<Bytef> uncompressed1(srcLen);
    uLongf uncomp1Len = srcLen;
    ret = uncompress(uncompressed1.data(), &uncomp1Len,
                     compressed1.data(), comp1Len);
    if (ret != Z_OK || uncomp1Len != srcLen) return -2;

    if (std::memcmp(uncompressed1.data(), original.data(), srcLen) != 0) return -3;

    // verify CRC matches
    uLong crc_uncomp1 = crc32(0L, Z_NULL, 0);
    crc_uncomp1 = crc32(crc_uncomp1, uncompressed1.data(), uncomp1Len);
    if (crc_uncomp1 != crc_orig) return -4;

    // 3) Manual deflate (streaming) and inflate (streaming)
    // Deflate
    z_stream defstr;
    std::memset(&defstr, 0, sizeof(defstr));
    ret = deflateInit(&defstr, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    defstr.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(original.data()));
    defstr.avail_in = srcLen;

    std::vector<Bytef> compressed2;
    const size_t CHUNK = 128;
    std::vector<Bytef> outbuf(CHUNK);

    int deflate_ret = Z_OK;
    while (deflate_ret != Z_STREAM_END) {
        defstr.next_out = outbuf.data();
        defstr.avail_out = static_cast<uInt>(outbuf.size());
        deflate_ret = deflate(&defstr, Z_FINISH); // finish in one pass because all input is provided
        if (deflate_ret != Z_OK && deflate_ret != Z_STREAM_END && deflate_ret != Z_BUF_ERROR) {
            deflateEnd(&defstr);
            return -6;
        }
        size_t have = outbuf.size() - defstr.avail_out;
        compressed2.insert(compressed2.end(), outbuf.data(), outbuf.data() + have);
        if (deflate_ret == Z_BUF_ERROR && defstr.avail_out == 0) {
            // continue to drain
            continue;
        }
        if (deflate_ret == Z_STREAM_END) break;
    }
    deflateEnd(&defstr);

    // Inflate
    z_stream infstr;
    std::memset(&infstr, 0, sizeof(infstr));
    ret = inflateInit(&infstr);
    if (ret != Z_OK) return -7;

    infstr.next_in = compressed2.empty() ? Z_NULL : compressed2.data();
    infstr.avail_in = static_cast<uInt>(compressed2.size());

    std::vector<Bytef> uncompressed2;
    std::vector<Bytef> in_out_buf(CHUNK);
    int inflate_ret = Z_OK;
    while (inflate_ret != Z_STREAM_END) {
        infstr.next_out = in_out_buf.data();
        infstr.avail_out = static_cast<uInt>(in_out_buf.size());
        inflate_ret = inflate(&infstr, Z_NO_FLUSH);
        if (inflate_ret != Z_OK && inflate_ret != Z_STREAM_END && inflate_ret != Z_BUF_ERROR) {
            inflateEnd(&infstr);
            return -8;
        }
        size_t have = in_out_buf.size() - infstr.avail_out;
        uncompressed2.insert(uncompressed2.end(), in_out_buf.data(), in_out_buf.data() + have);
        if (inflate_ret == Z_BUF_ERROR && infstr.avail_out == 0) {
            // continue; more output needed
            continue;
        }
        if (inflate_ret == Z_STREAM_END) break;
        // If avail_in becomes zero but not finished, break to avoid infinite loop
        if (infstr.avail_in == 0 && inflate_ret == Z_BUF_ERROR) break;
    }
    inflateEnd(&infstr);

    if (uncompressed2.size() != srcLen) return -9;
    if (std::memcmp(uncompressed2.data(), original.data(), srcLen) != 0) return -10;

    // 4) Adler-32 of the decompressed data (streaming-friendly checksum)
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, uncompressed2.data(), static_cast<uInt>(uncompressed2.size()));
    if (adler == 0) {
        // 0 is a valid adler for empty data; here non-empty, so 0 would be suspicious
        return -11;
    }

    // If all checks passed, return 66
    return 66;
}