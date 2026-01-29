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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress
    std::string original;
    for (int i = 0; i < 64; ++i) {
        original += "The quick brown fox jumps over the lazy dog. ";
    }
    const size_t origLen = original.size();

    // 1) Use deflateInit / deflate / deflateEnd (stream API)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -1;

    // allocate output buffer using compressBound for an upper bound
    uLong bound = compressBound(static_cast<uLong>(origLen));
    std::vector<unsigned char> compBuf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(origLen);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(bound);

    // Run deflate until stream end
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&defstream);
        return -2;
    }
    // It's possible multiple calls are needed; loop until Z_STREAM_END
    while (ret != Z_STREAM_END) {
        // expand buffer if needed (shouldn't typically be needed due to compressBound)
        size_t oldSize = compBuf.size();
        compBuf.resize(oldSize * 2);
        defstream.next_out = compBuf.data() + (oldSize - defstream.avail_out);
        defstream.avail_out = static_cast<uInt>(oldSize); // new extra space
        ret = deflate(&defstream, Z_FINISH);
        if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
            deflateEnd(&defstream);
            return -3;
        }
    }

    size_t compressedSize = compBuf.size() - defstream.avail_out;
    deflateEnd(&defstream);

    // 2) Use inflateInit / inflate / inflateEnd (stream API)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -4;

    std::vector<unsigned char> decompBuf(origLen + 16); // slight slack
    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compressedSize);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    // Run inflate until stream end
    ret = inflate(&infstream, Z_FINISH);
    while (ret != Z_STREAM_END) {
        if (ret == Z_OK || ret == Z_BUF_ERROR) {
            // Expand output buffer and continue
            size_t oldOut = decompBuf.size();
            decompBuf.resize(oldOut * 2);
            infstream.next_out = decompBuf.data() + (oldOut - infstream.avail_out);
            infstream.avail_out = static_cast<uInt>(oldOut);
            ret = inflate(&infstream, Z_FINISH);
            continue;
        } else {
            inflateEnd(&infstream);
            return -5;
        }
    }

    size_t decompressedSize = decompBuf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    if (decompressedSize != origLen) return -6;
    if (std::memcmp(decompBuf.data(), original.data(), origLen) != 0) return -7;

    // 3) Use convenience APIs compress() and uncompress()
    uLong cBound = compressBound(static_cast<uLong>(origLen));
    std::vector<unsigned char> comp2(cBound);
    uLong comp2Len = cBound;
    ret = compress(comp2.data(), &comp2Len,
                   reinterpret_cast<const Bytef*>(original.data()),
                   static_cast<uLong>(origLen));
    if (ret != Z_OK) return -8;

    std::vector<unsigned char> decomp2(origLen);
    uLong decomp2Len = static_cast<uLong>(origLen);
    ret = uncompress(decomp2.data(), &decomp2Len, comp2.data(), comp2Len);
    if (ret != Z_OK) return -9;
    if (decomp2Len != origLen) return -10;
    if (std::memcmp(decomp2.data(), original.data(), origLen) != 0) return -11;

    // 4) Check adler32 and crc32 checksums
    uLong a = adler32(0L, Z_NULL, 0);
    a = adler32(a, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(origLen));
    uLong c = crc32(0L, Z_NULL, 0);
    c = crc32(c, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(origLen));
    // Basic sanity: checksums should be non-zero for non-empty data
    if (a == 0 || c == 0) return -12;

    // 5) Use gz* API to write and read a .gz file
    const char* fname = "test_zlib_api_sequence.gz";
    gzFile gz = gzopen(fname, "wb");
    if (!gz) return -13;
    int written = gzwrite(gz, original.data(), static_cast<unsigned int>(origLen));
    if (written != static_cast<int>(origLen)) {
        gzclose(gz);
        return -14;
    }
    gzclose(gz);

    // Read back
    gz = gzopen(fname, "rb");
    if (!gz) {
        std::remove(fname);
        return -15;
    }
    std::vector<char> gzreadBuf(origLen);
    int read = gzread(gz, gzreadBuf.data(), static_cast<unsigned int>(origLen));
    gzclose(gz);
    // remove the file after test
    std::remove(fname);

    if (read != static_cast<int>(origLen)) return -16;
    if (std::memcmp(gzreadBuf.data(), original.data(), origLen) != 0) return -17;

    // If we reached here, all zlib API sequences worked correctly
    return 66;
}