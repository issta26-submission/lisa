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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Returns 66 on success, non-66 negative values on failure.
int test_zlib_api_sequence() {
    // Basic check of zlib version (usage of zlibVersion()).
    const char* ver = zlibVersion();
    if (!ver) return -1;

    // Prepare sample input data (repeated to make it compressible).
    std::string input;
    for (int i = 0; i < 64; ++i) input += "The quick brown fox jumps over the lazy dog. ";
    uLong sourceLen = static_cast<uLong>(input.size());

    // 1) Use compressBound + compress2 to compress in one call.
    uLongf compLenF = compressBound(sourceLen);
    std::vector<Bytef> compBuf(static_cast<size_t>(compLenF));
    int ret = compress2(compBuf.data(), &compLenF,
                        reinterpret_cast<const Bytef*>(input.data()), sourceLen,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -2;
    compBuf.resize(static_cast<size_t>(compLenF));

    // 2) Use uncompress to decompress and verify exact equality.
    uLongf uncomprLenF = sourceLen;
    std::vector<Bytef> uncomprBuf(static_cast<size_t>(uncomprLenF));
    ret = uncompress(uncomprBuf.data(), &uncomprLenF, compBuf.data(),
                     static_cast<uLong>(compBuf.size()));
    if (ret != Z_OK) return -3;
    if (uncomprLenF != sourceLen) return -4;
    if (std::memcmp(uncomprBuf.data(), input.data(), sourceLen) != 0) return -5;

    // 3) Demonstrate low-level streaming API for deflate.
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -6;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    std::vector<Bytef> streamDefOut;
    const size_t outChunk = 256;
    int flush_mode;
    do {
        Bytef out[outChunk];
        defstream.next_out = out;
        defstream.avail_out = static_cast<uInt>(outChunk);

        flush_mode = (defstream.avail_in ? Z_NO_FLUSH : Z_FINISH);
        ret = deflate(&defstream, flush_mode);
        if (ret == Z_STREAM_ERROR) { deflateEnd(&defstream); return -7; }

        size_t have = outChunk - defstream.avail_out;
        if (have > 0) streamDefOut.insert(streamDefOut.end(), out, out + have);
    } while (ret != Z_STREAM_END);

    // Cleanup deflate state.
    deflateEnd(&defstream);

    // 4) Demonstrate low-level streaming API for inflate.
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -8;

    infstream.next_in = streamDefOut.data();
    infstream.avail_in = static_cast<uInt>(streamDefOut.size());

    std::vector<Bytef> streamInfOut;
    do {
        Bytef out[outChunk];
        infstream.next_out = out;
        infstream.avail_out = static_cast<uInt>(outChunk);

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) { inflateEnd(&infstream); return -9; }
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&infstream);
            return -10;
        }
        size_t have = outChunk - infstream.avail_out;
        if (have > 0) streamInfOut.insert(streamInfOut.end(), out, out + have);
    } while (ret != Z_STREAM_END);

    // Cleanup inflate state.
    inflateEnd(&infstream);

    if (streamInfOut.size() != input.size()) return -11;
    if (std::memcmp(streamInfOut.data(), input.data(), input.size()) != 0) return -12;

    // 5) Compute checksums: crc32 and adler32.
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()),
                static_cast<uInt>(input.size()));
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()),
                  static_cast<uInt>(input.size()));
    // Not asserting specific values, just ensuring the calls succeed and return something.
    (void)crc;
    (void)adl;

    // All checks passed.
    return 66;
}