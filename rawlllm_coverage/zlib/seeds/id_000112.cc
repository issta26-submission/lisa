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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when copied into a file.
    // (Preprocessor directives are processed before compilation; including inside the function is valid.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>

    // Prepare a sample input string (repeated to make it reasonably compressible)
    std::string original;
    for (int i = 0; i < 16; ++i) original += "The quick brown fox jumps over the lazy dog. ";

    // 1) Use compress() / uncompress() convenience APIs
    uLong origSize = static_cast<uLong>(original.size());
    uLong compBound = compressBound(origSize);
    std::vector<Bytef> compBuf(compBound);

    int ret = compress(compBuf.data(), &compBound,
                       reinterpret_cast<const Bytef*>(original.data()), origSize);
    if (ret != Z_OK) return -1; // compress failed

    // uncompress into a buffer of expected size
    std::vector<Bytef> unCompBuf(origSize);
    uLong unCompSize = origSize;
    ret = uncompress(unCompBuf.data(), &unCompSize, compBuf.data(), compBound);
    if (ret != Z_OK) return -2; // uncompress failed
    if (unCompSize != origSize) return -3;
    if (std::memcmp(unCompBuf.data(), original.data(), origSize) != 0) return -4;

    // 2) Streaming deflate() / inflate() sequence
    // Deflate (compress) in streaming mode
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -5;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(origSize); // safe for our small buffer

    uLong streamCompBound = deflateBound(&defstream, origSize);
    std::vector<Bytef> streamCompBuf(streamCompBound);
    defstream.next_out = streamCompBuf.data();
    defstream.avail_out = static_cast<uInt>(streamCompBound);

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&defstream);
        return -6;
    }
    // number of bytes produced
    uLong streamCompSize = streamCompBound - defstream.avail_out;
    deflateEnd(&defstream);

    // Inflate (decompress) in streaming mode
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -7;

    infstream.next_in = streamCompBuf.data();
    infstream.avail_in = static_cast<uInt>(streamCompSize);

    std::vector<Bytef> streamUncompBuf(origSize);
    infstream.next_out = streamUncompBuf.data();
    infstream.avail_out = static_cast<uInt>(origSize);

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&infstream);
        return -8;
    }
    uLong streamUncompSize = origSize - infstream.avail_out;
    inflateEnd(&infstream);

    if (streamUncompSize != origSize) return -9;
    if (std::memcmp(streamUncompBuf.data(), original.data(), origSize) != 0) return -10;

    // 3) Check checksums: crc32 and adler32 should match for identical data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()), origSize);
    uLong crc_uncomp = crc32(0L, Z_NULL, 0);
    crc_uncomp = crc32(crc_uncomp, streamUncompBuf.data(), streamUncompSize);
    if (crc_orig != crc_uncomp) return -11;

    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(original.data()), origSize);
    uLong adler_uncomp = adler32(0L, Z_NULL, 0);
    adler_uncomp = adler32(adler_uncomp, streamUncompBuf.data(), streamUncompSize);
    if (adler_orig != adler_uncomp) return -12;

    // 4) Query zlib version string
    const char* ver = zlibVersion();
    if (ver == nullptr || ver[0] == '\0') return -13;

    // If we reach here, all zlib API sequences executed and validated successfully.
    return 66;
}