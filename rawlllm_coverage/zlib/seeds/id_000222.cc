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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function exercises a variety of zlib APIs:
    // - compress2 / uncompress (simple API)
    // - deflateInit / deflate / deflateEnd (streaming compress)
    // - inflateInit / inflate / inflateEnd (streaming decompress)
    // - crc32 and adler32 checksums
    //
    // Note: This function assumes the including translation unit has:
    // #include <zlib.h>
    // #include <vector>
    // #include <cstring>
    //
    // It returns 66 on success; non-66 indicates an error at some step.

    const char* plain = "The quick brown fox jumps over the lazy dog";
    uLong plain_len = static_cast<uLong>(std::strlen(plain));

    // 1) Simple compress/uncompress using compress2 / uncompress
    uLong bound = compressBound(plain_len);
    std::vector<unsigned char> compBuf(bound);
    uLongf compSize = bound;
    int ret = compress2(compBuf.data(), &compSize,
                        reinterpret_cast<const Bytef*>(plain), plain_len,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 1;

    // uncompress back
    std::vector<unsigned char> unpBuf(plain_len);
    uLongf unpSize = plain_len;
    ret = uncompress(unpBuf.data(), &unpSize, compBuf.data(), compSize);
    if (ret != Z_OK) return 2;
    if (unpSize != plain_len) return 3;
    if (std::memcmp(unpBuf.data(), plain, plain_len) != 0) return 4;

    // 2) Streaming compress using deflate*
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    // Prepare output buffer
    std::vector<unsigned char> compBuf2(compressBound(plain_len));
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(plain));
    defstrm.avail_in = plain_len;
    defstrm.next_out = compBuf2.data();
    defstrm.avail_out = static_cast<uInt>(compBuf2.size());

    ret = deflate(&defstrm, Z_FINISH); // compress all input
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstrm);
        return 6;
    }
    uLongf compSize2 = static_cast<uLongf>(compBuf2.size() - defstrm.avail_out);
    deflateEnd(&defstrm);

    // 3) Streaming inflate using inflate*
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) return 7;

    std::vector<unsigned char> outBuf2(plain_len);
    infstrm.next_in = compBuf2.data();
    infstrm.avail_in = static_cast<uInt>(compSize2);
    infstrm.next_out = outBuf2.data();
    infstrm.avail_out = static_cast<uInt>(outBuf2.size());

    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstrm);
        return 8;
    }
    uLongf outSize2 = static_cast<uLongf>(outBuf2.size() - infstrm.avail_out);
    inflateEnd(&infstrm);

    if (outSize2 != plain_len) return 9;
    if (std::memcmp(outBuf2.data(), plain, plain_len) != 0) return 10;

    // 4) Checksum calculations: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(plain), plain_len);
    // compute via adler32 as well
    uLong ad = adler32(0L, Z_NULL, 0);
    ad = adler32(ad, reinterpret_cast<const Bytef*>(plain), plain_len);
    // Basic sanity checks: they should be non-zero for non-empty input
    if (plain_len > 0 && (crc == 0 || ad == 0)) return 11;

    // 5) Also demonstrate compressBound usage was meaningful
    if (compSize > bound) return 12; // should not exceed compressBound

    // If we reached here, all exercised zlib APIs worked as expected.
    return 66;
}