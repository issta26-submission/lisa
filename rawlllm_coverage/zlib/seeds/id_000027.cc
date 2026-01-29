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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare input data
    const std::string input = std::string("The quick brown fox jumps over the lazy dog. ") +
                              std::string("Repeat to increase size: ");
    // make it reasonably large for streaming
    std::string src;
    for (int i = 0; i < 64; ++i) src += input; // ~64 * ~70 bytes

    const uLong sourceLen = static_cast<uLong>(src.size());
    if (sourceLen == 0) return -1;

    // 1) Simple one-shot compress()/uncompress() usage
    uLong bound = compressBound(sourceLen);
    std::vector<Bytef> comp1(bound);
    uLongf comp1Len = static_cast<uLongf>(bound);
    int zret = compress(comp1.data(), &comp1Len,
                        reinterpret_cast<const Bytef*>(src.data()), sourceLen);
    if (zret != Z_OK) return -2;

    // Uncompress and verify
    std::vector<Bytef> un1(sourceLen);
    uLongf un1Len = sourceLen;
    zret = uncompress(un1.data(), &un1Len, comp1.data(), comp1Len);
    if (zret != Z_OK) return -3;
    if (un1Len != sourceLen) return -4;
    if (std::memcmp(un1.data(), src.data(), sourceLen) != 0) return -5;

    // 2) Streaming deflate (deflateInit / deflate / deflateEnd)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    zret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (zret != Z_OK) return -6;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src.data()));
    defstream.avail_in = static_cast<uInt>(sourceLen); // avail_in is uInt
    std::vector<Bytef> comp2;
    const size_t outChunk = 256;
    std::vector<Bytef> outbuf(outChunk);

    // We call deflate with Z_FINISH to finish the stream (we provided all input)
    for (;;) {
        defstream.next_out = outbuf.data();
        defstream.avail_out = static_cast<uInt>(outChunk);
        zret = deflate(&defstream, Z_FINISH); // finish the compression
        if (zret != Z_OK && zret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -7;
        }
        size_t produced = outChunk - defstream.avail_out;
        comp2.insert(comp2.end(), outbuf.data(), outbuf.data() + produced);
        if (zret == Z_STREAM_END) break;
        // otherwise loop to get more output
    }

    deflateEnd(&defstream); // clean up

    // 3) Streaming inflate (inflateInit / inflate / inflateEnd)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    zret = inflateInit(&infstream);
    if (zret != Z_OK) return -8;

    infstream.next_in = comp2.empty() ? Z_NULL : comp2.data();
    infstream.avail_in = comp2.size() > 0 ? static_cast<uInt>(comp2.size()) : 0;
    std::vector<Bytef> out2;
    outbuf.assign(outChunk, 0);

    for (;;) {
        infstream.next_out = outbuf.data();
        infstream.avail_out = static_cast<uInt>(outChunk);
        zret = inflate(&infstream, Z_NO_FLUSH);
        if (zret != Z_OK && zret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -9;
        }
        size_t produced = outChunk - infstream.avail_out;
        out2.insert(out2.end(), outbuf.data(), outbuf.data() + produced);
        if (zret == Z_STREAM_END) break;
        if (infstream.avail_in == 0 && infstream.avail_out != 0) {
            // No more input and no more output produced -> break to avoid infinite loop
            break;
        }
    }

    inflateEnd(&infstream);

    // Verify streaming inflate result equals original
    if (out2.size() != sourceLen) return -10;
    if (std::memcmp(out2.data(), src.data(), sourceLen) != 0) return -11;

    // 4) Compute checksums: crc32 and adler32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(src.data()), sourceLen);

    uLong ad = adler32(0L, Z_NULL, 0);
    ad = adler32(ad, reinterpret_cast<const Bytef*>(src.data()), sourceLen);

    // Basic sanity: checksums should be non-zero for non-empty input
    if (crc == 0 || ad == 0) return -12;

    // If we reach here, all zlib API sequences succeeded
    return 66;
}