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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers (allowed inside function scope; preprocessor directives are handled before compilation)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Prepare sample input (repeat to make it compressible)
    std::string original;
    for (int i = 0; i < 64; ++i) original += "The quick brown fox jumps over the lazy dog. ";
    const Bytef* src = reinterpret_cast<const Bytef*>(original.data());
    uLong sourceLen = static_cast<uLong>(original.size());

    // 1) Use deflate (stream API) to compress
    z_stream deflateStream;
    std::memset(&deflateStream, 0, sizeof(deflateStream));
    deflateStream.zalloc = Z_NULL;
    deflateStream.zfree = Z_NULL;
    deflateStream.opaque = Z_NULL;

    int ret = deflateInit(&deflateStream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    deflateStream.next_in = const_cast<Bytef*>(src);
    deflateStream.avail_in = sourceLen;

    uLong bound = compressBound(sourceLen);
    std::vector<Bytef> compressed(bound);
    deflateStream.next_out = compressed.data();
    deflateStream.avail_out = static_cast<uInt>(bound);

    // Perform compression; one call to deflate with Z_FINISH is sufficient because output buffer is large enough
    ret = deflate(&deflateStream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&deflateStream);
        return -2;
    }
    uLong compressedSize = bound - deflateStream.avail_out;
    deflateEnd(&deflateStream);

    // 2) Use inflate (stream API) to decompress
    z_stream inflateStream;
    std::memset(&inflateStream, 0, sizeof(inflateStream));
    inflateStream.zalloc = Z_NULL;
    inflateStream.zfree = Z_NULL;
    inflateStream.opaque = Z_NULL;

    inflateStream.next_in = compressed.data();
    inflateStream.avail_in = static_cast<uInt>(compressedSize);

    std::vector<Bytef> decompressed(sourceLen);
    inflateStream.next_out = decompressed.data();
    inflateStream.avail_out = static_cast<uInt>(sourceLen);

    ret = inflateInit(&inflateStream);
    if (ret != Z_OK) {
        return -3;
    }

    // Loop until stream end or error (should finish in one call here)
    while (true) {
        ret = inflate(&inflateStream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            inflateEnd(&inflateStream);
            return -4;
        }
        // If more output space was needed, that would be handled here (not needed in this test)
    }
    uLong decompressedSize = sourceLen - inflateStream.avail_out;
    inflateEnd(&inflateStream);

    if (decompressedSize != sourceLen) {
        return -5;
    }
    if (std::memcmp(decompressed.data(), original.data(), sourceLen) != 0) {
        return -6;
    }

    // 3) Use the convenience API compress2 / uncompress as an additional check
    uLongf comp2Bound = compressBound(sourceLen);
    std::vector<Bytef> comp2(comp2Bound);
    uLongf comp2Size = comp2Bound;
    ret = compress2(comp2.data(), &comp2Size, src, sourceLen, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return -7;
    }

    std::vector<Bytef> uncomp2(sourceLen);
    uLongf uncomp2Size = sourceLen;
    ret = uncompress(uncomp2.data(), &uncomp2Size, comp2.data(), comp2Size);
    if (ret != Z_OK) {
        return -8;
    }
    if (uncomp2Size != sourceLen) return -9;
    if (std::memcmp(uncomp2.data(), original.data(), sourceLen) != 0) return -10;

    // If all steps succeeded, return the required success code
    return 66;
}