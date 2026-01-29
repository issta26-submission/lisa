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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so this function is self-contained when pasted into a file.
    // (Preprocessor directives are allowed anywhere in the translation unit.)
    #include <zlib.h>
    #include <cstring>

    // Example input data to compress/decompress
    const char *src = "The quick brown fox jumps over the lazy dog. "
                      "This string is repeated to give some compressible data. "
                      "The quick brown fox jumps over the lazy dog. "
                      "The quick brown fox jumps over the lazy dog.";
    const unsigned long srcLen = static_cast<unsigned long>(std::strlen(src));

    // 1) Compute checksums (crc32 and adler32)
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(src), srcLen);

    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, reinterpret_cast<const Bytef*>(src), srcLen);

    // 2) Use compress() (simple one-shot API)
    uLong bound = compressBound(srcLen);
    Bytef *oneShotComp = new (std::nothrow) Bytef[bound];
    if (!oneShotComp) return -1;
    uLongf oneShotCompSize = bound;
    int ret = compress(oneShotComp, &oneShotCompSize,
                       reinterpret_cast<const Bytef*>(src), srcLen);
    if (ret != Z_OK) {
        delete [] oneShotComp;
        return -2;
    }

    // 3) Streamed deflate (using z_stream / deflate)
    z_stream defstrm = {}; // zero-initialize
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        delete [] oneShotComp;
        return -3;
    }

    // Provide input and an output buffer large enough
    const uInt outBufSize = static_cast<uInt>(srcLen + 1024);
    Bytef *streamComp = new (std::nothrow) Bytef[outBufSize];
    if (!streamComp) {
        deflateEnd(&defstrm);
        delete [] oneShotComp;
        return -4;
    }

    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstrm.avail_in = static_cast<uInt>(srcLen);
    defstrm.next_out = streamComp;
    defstrm.avail_out = outBufSize;

    // Call deflate until stream ends
    while ((ret = deflate(&defstrm, Z_FINISH)) == Z_OK) {
        // If output buffer exhausted, this would need resizing; our buffer is big enough for this input
        if (defstrm.avail_out == 0) break;
    }
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&defstrm);
        delete [] streamComp;
        delete [] oneShotComp;
        return -5;
    }
    uLong streamCompSize = outBufSize - defstrm.avail_out;
    deflateEnd(&defstrm);

    // 4) Demonstrate deflateParams, deflateTune and deflateCopy
    z_stream d1 = {};
    ret = deflateInit(&d1, Z_BEST_SPEED);
    if (ret != Z_OK) {
        delete [] streamComp;
        delete [] oneShotComp;
        return -6;
    }
    // Change params on the fly (allowed before heavy usage)
    (void)deflateParams(&d1, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    // Tune heuristic parameters (values are illustrative)
    (void)deflateTune(&d1, 64, 258, 64, 128);

    z_stream d2 = {};
    ret = deflateCopy(&d2, &d1);
    // After copying, both streams should be ended to free resources
    deflateEnd(&d1);
    if (ret == Z_OK) deflateEnd(&d2);

    // 5) Streamed inflate (using the one-shot compressed data produced earlier)
    z_stream instrm = {};
    ret = inflateInit(&instrm);
    if (ret != Z_OK) {
        delete [] streamComp;
        delete [] oneShotComp;
        return -7;
    }

    // Allocate buffer for decompressed data
    Bytef *decompBuf = new (std::nothrow) Bytef[srcLen + 10];
    if (!decompBuf) {
        inflateEnd(&instrm);
        delete [] streamComp;
        delete [] oneShotComp;
        return -8;
    }

    instrm.next_in = oneShotComp;
    instrm.avail_in = static_cast<uInt>(oneShotCompSize);
    instrm.next_out = decompBuf;
    instrm.avail_out = static_cast<uInt>(srcLen + 10);

    // Inflate until stream end
    while ((ret = inflate(&instrm, Z_NO_FLUSH)) == Z_OK) {
        if (instrm.avail_out == 0) break; // normally would grow buffer
    }
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&instrm);
        delete [] decompBuf;
        delete [] streamComp;
        delete [] oneShotComp;
        return -9;
    }
    uLong decompSize = (srcLen + 10) - instrm.avail_out;
    inflateEnd(&instrm);

    // 6) One-shot uncompress() to double-check
    Bytef *uncomp2 = new (std::nothrow) Bytef[srcLen + 10];
    if (!uncomp2) {
        delete [] decompBuf;
        delete [] streamComp;
        delete [] oneShotComp;
        return -10;
    }
    uLongf uncomp2Size = srcLen + 10;
    ret = uncompress(uncomp2, &uncomp2Size, oneShotComp, oneShotCompSize);
    if (ret != Z_OK) {
        delete [] uncomp2;
        delete [] decompBuf;
        delete [] streamComp;
        delete [] oneShotComp;
        return -11;
    }

    // 7) Verify decompressed data matches original
    bool ok = (decompSize == srcLen) &&
              (std::memcmp(decompBuf, src, srcLen) == 0) &&
              (uncomp2Size == srcLen) &&
              (std::memcmp(uncomp2, src, srcLen) == 0);

    // Clean up
    delete [] uncomp2;
    delete [] decompBuf;
    delete [] streamComp;
    delete [] oneShotComp;

    if (!ok) return -12;

    // Optionally inspect version and error strings (no-op here, just exercise API)
    const char *version = zlibVersion();
    (void)version;
    // zError takes an int error code; we can call it for a known code like Z_OK
    const char *okStr = zError(Z_OK);
    (void)okStr;

    // All zlib APIs used in various ways succeeded; return the requested success value.
    return 66;
}