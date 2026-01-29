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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {

    const std::string input =
        "The quick brown fox jumps over the lazy dog. zlib API sequence test. "
        "Repeat to increase size. The quick brown fox jumps over the lazy dog.";
    uLong srcLen = static_cast<uLong>(input.size());

    // 1) Simple one-shot compression using compress2 + one-shot uncompress
    uLong compBound = compressBound(srcLen);
    std::vector<Bytef> compBuf(compBound);
    int zret = compress2(compBuf.data(), &compBound,
                         reinterpret_cast<const Bytef*>(input.data()), srcLen,
                         Z_BEST_COMPRESSION);
    if (zret != Z_OK) return 1;
    uLong compSize = compBound;

    std::vector<Bytef> uncmpBuf(srcLen);
    uLong uncmpLen = srcLen;
    zret = uncompress(uncmpBuf.data(), &uncmpLen, compBuf.data(), compSize);
    if (zret != Z_OK) return 2;
    if (uncmpLen != srcLen) return 3;
    if (std::memcmp(uncmpBuf.data(), input.data(), srcLen) != 0) return 4;

    // 2) Streaming API: deflate (streaming) then inflate (streaming)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    zret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (zret != Z_OK) return 5;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = srcLen;

    uLong streamOutBound = compressBound(srcLen);
    std::vector<Bytef> streamCompBuf(streamOutBound);
    defstream.next_out = streamCompBuf.data();
    defstream.avail_out = streamOutBound;

    zret = deflate(&defstream, Z_FINISH);
    if (zret != Z_STREAM_END) { deflateEnd(&defstream); return 6; }
    uLong deflatedBytes = streamOutBound - defstream.avail_out;
    deflateEnd(&defstream);

    // Inflate the streamed data
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    zret = inflateInit(&infstream);
    if (zret != Z_OK) return 7;

    infstream.next_in = streamCompBuf.data();
    infstream.avail_in = deflatedBytes;

    std::vector<Bytef> streamUncompBuf(srcLen);
    infstream.next_out = streamUncompBuf.data();
    infstream.avail_out = srcLen;

    zret = inflate(&infstream, Z_FINISH);
    if (zret != Z_STREAM_END) { inflateEnd(&infstream); return 8; }
    uLong inflatedBytes = srcLen - infstream.avail_out;
    inflateEnd(&infstream);

    if (inflatedBytes != srcLen) return 9;
    if (std::memcmp(streamUncompBuf.data(), input.data(), srcLen) != 0) return 10;

    // 3) Checksums: adler32 and crc32 usage and cross-check
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), srcLen);

    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), srcLen);

    uLong crc_inflated = crc32(0L, Z_NULL, 0);
    crc_inflated = crc32(crc_inflated, streamUncompBuf.data(), inflatedBytes);

    if (crc != crc_inflated) return 11;

    // All checks passed
    return 66;
}