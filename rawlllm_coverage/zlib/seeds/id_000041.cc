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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// A self-contained test function that exercises a sequence of zlib APIs.
// Returns 66 on success, non-66 on failure.
int test_zlib_api_sequence() {
    // Example input - make it a bit larger by repeating
    std::string input;
    for (int i = 0; i < 50; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    uLong sourceLen = static_cast<uLong>(input.size());

    // 1) Compute checksums (crc32 and adler32)
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input.size()));

    uLong adler_before = adler32(0L, Z_NULL, 0);
    adler_before = adler32(adler_before, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input.size()));

    // 2) Use compress() (simple one-shot)
    uLongf compBound = compressBound(sourceLen);
    std::vector<unsigned char> compBuf(compBound);
    uLongf compSize = compBound;
    int ret = compress(compBuf.data(), &compSize,
                       reinterpret_cast<const Bytef*>(input.data()), sourceLen);
    if (ret != Z_OK) return -1;

    // 3) Use uncompress() to restore and verify
    std::vector<unsigned char> uncompBuf(sourceLen);
    uLongf uncompSize = sourceLen;
    ret = uncompress(uncompBuf.data(), &uncompSize, compBuf.data(), compSize);
    if (ret != Z_OK) return -2;
    if (uncompSize != sourceLen) return -3;
    if (std::memcmp(uncompBuf.data(), input.data(), sourceLen) != 0) return -4;

    // 4) Use compress2() with a different compression level
    std::vector<unsigned char> comp2Buf(compBound);
    uLongf comp2Size = compBound;
    ret = compress2(comp2Buf.data(), &comp2Size,
                    reinterpret_cast<const Bytef*>(input.data()), sourceLen,
                    Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -5;

    // 5) Uncompress the compress2 output and verify
    std::vector<unsigned char> uncomp2Buf(sourceLen);
    uLongf uncomp2Size = sourceLen;
    ret = uncompress(uncomp2Buf.data(), &uncomp2Size, comp2Buf.data(), comp2Size);
    if (ret != Z_OK) return -6;
    if (uncomp2Size != sourceLen) return -7;
    if (std::memcmp(uncomp2Buf.data(), input.data(), sourceLen) != 0) return -8;

    // 6) Use the streaming API: deflateInit/deflate/deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -9;

    // Prepare input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    // Output buffer large enough using compressBound
    std::vector<unsigned char> streamCompBuf(compBound);
    defstream.next_out = streamCompBuf.data();
    defstream.avail_out = static_cast<uInt>(streamCompBuf.size());

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -10;
    }
    uLongf streamCompSize = streamCompBuf.size() - defstream.avail_out;
    deflateEnd(&defstream);

    // 7) Use the streaming API: inflateInit/inflate/inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -11;

    infstream.next_in = streamCompBuf.data();
    infstream.avail_in = static_cast<uInt>(streamCompSize);

    std::vector<unsigned char> streamUncompBuf(sourceLen);
    infstream.next_out = streamUncompBuf.data();
    infstream.avail_out = static_cast<uInt>(streamUncompBuf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -12;
    }
    uLongf streamUncompSize = streamUncompBuf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    if (streamUncompSize != sourceLen) return -13;
    if (std::memcmp(streamUncompBuf.data(), input.data(), sourceLen) != 0) return -14;

    // 8) Recompute checksums and verify they match the earlier values
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, reinterpret_cast<const Bytef*>(streamUncompBuf.data()), static_cast<uInt>(streamUncompBuf.size()));
    if (crc_after != crc_before) return -15;

    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, reinterpret_cast<const Bytef*>(streamUncompBuf.data()), static_cast<uInt>(streamUncompBuf.size()));
    if (adler_after != adler_before) return -16;

    // If we reached here, all tested API sequences worked and data validated
    return 66;
}