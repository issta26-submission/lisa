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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers inside the function so the snippet is self-contained.
    // (Including headers inside a function is legal C++ and keeps this example
    // standalone. In real code, prefer placing includes at file scope.)
    #include <zlib.h>
    #include <vector>
    #include <cstring>

    const char *input = "The quick brown fox jumps over the lazy dog";
    uLong sourceLen = static_cast<uLong>(std::strlen(input));

    // 1) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input), sourceLen);
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input), sourceLen);

    // 2) Deflate (compress) using z_stream API
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        deflateEnd(&defstream);
        return -1;
    }

    uLong bound = compressBound(sourceLen);
    std::vector<Bytef> compressed(bound);
    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input));
    defstream.avail_in = sourceLen;
    defstream.next_out = compressed.data();
    defstream.avail_out = static_cast<uInt>(bound); // avail_out is uInt

    // Use Z_FINISH since we provide the whole input and have enough output space.
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }
    uLong compressedSize = bound - defstream.avail_out;
    deflateEnd(&defstream);

    // 3) Inflate (decompress) using z_stream API and verify output matches input
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        inflateEnd(&infstream);
        return -3;
    }

    std::vector<Bytef> decompressed(sourceLen + 16); // a bit of padding
    infstream.next_in = compressed.data();
    infstream.avail_in = static_cast<uInt>(compressedSize);
    infstream.next_out = decompressed.data();
    infstream.avail_out = static_cast<uInt>(decompressed.size());

    // Inflate in a single call (enough output buffer allocated).
    ret = inflate(&infstream, Z_NO_FLUSH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&infstream);
        return -4;
    }
    uLong decompressedSize = static_cast<uLong>(decompressed.size()) - infstream.avail_out;
    inflateEnd(&infstream);

    if (decompressedSize != sourceLen) return -5;
    if (std::memcmp(decompressed.data(), input, sourceLen) != 0) return -6;

    // 4) Use the convenience compress2 / uncompress APIs and verify round-trip
    uLong comp2Bound = compressBound(sourceLen);
    std::vector<Bytef> comp2(comp2Bound);
    uLong comp2Len = comp2Bound;
    ret = compress2(comp2.data(), &comp2Len, reinterpret_cast<const Bytef*>(input), sourceLen, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -7;

    std::vector<Bytef> decomp2(sourceLen + 8);
    uLong decomp2Len = static_cast<uLong>(decomp2.size());
    ret = uncompress(decomp2.data(), &decomp2Len, comp2.data(), comp2Len);
    if (ret != Z_OK) return -8;
    if (decomp2Len != sourceLen) return -9;
    if (std::memcmp(decomp2.data(), input, sourceLen) != 0) return -10;

    // 5) Demonstrate deflateReset/inflateReset by reusing streams:
    // For this demo we initialize new streams and immediately reset them to show the API.
    z_stream dreset;
    std::memset(&dreset, 0, sizeof(dreset));
    dreset.zalloc = Z_NULL;
    dreset.zfree = Z_NULL;
    dreset.opaque = Z_NULL;
    ret = deflateInit(&dreset, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) { deflateEnd(&dreset); return -11; }
    ret = deflateReset(&dreset);
    if (ret != Z_OK) { deflateEnd(&dreset); return -12; }
    deflateEnd(&dreset);

    z_stream ireset;
    std::memset(&ireset, 0, sizeof(ireset));
    ireset.zalloc = Z_NULL;
    ireset.zfree = Z_NULL;
    ireset.opaque = Z_NULL;
    ret = inflateInit(&ireset);
    if (ret != Z_OK) { inflateEnd(&ireset); return -13; }
    ret = inflateReset(&ireset);
    if (ret != Z_OK) { inflateEnd(&ireset); return -14; }
    inflateEnd(&ireset);

    // If we reach here, all API sequences succeeded.
    (void)crc; // silence unused-variable warnings in builds where not inspected
    (void)adl;

    return 66;
}