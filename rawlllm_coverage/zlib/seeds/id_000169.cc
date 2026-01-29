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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when pasted alone.
    // (Preprocessor directives are allowed inside a function scope.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <cstdio>

    // Original data to compress
    const char* original = "The quick brown fox jumped over the lazy dog - zlib test.";
    const uLong origLen = static_cast<uLong>(std::strlen(original));

    // 1) Simple compress/uncompress (one-shot API)
    uLong compBound = compressBound(origLen);
    std::vector<Bytef> compBuf(compBound);
    uLong compLen = compBound;

    int ret = compress(compBuf.data(), &compLen, reinterpret_cast<const Bytef*>(original), origLen);
    if (ret != Z_OK) return -1; // compress failed

    // Decompress with uncompress()
    std::vector<Bytef> decompBuf(origLen + 1); // +1 for safety / null-termination
    uLong decompLen = static_cast<uLong>(decompBuf.size());
    ret = uncompress(decompBuf.data(), &decompLen, compBuf.data(), compLen);
    if (ret != Z_OK) return -2; // uncompress failed

    if (decompLen != origLen) return -3; // size mismatch
    if (std::memcmp(decompBuf.data(), original, origLen) != 0) return -4; // content mismatch

    // 2) Streaming deflate (z_stream) and inflate
    // Deflate (compress) using z_stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    defstream.avail_in = static_cast<uInt>(origLen);

    // Prepare output buffer large enough
    uLong streamOutBound = compressBound(origLen);
    std::vector<Bytef> streamCompBuf(streamOutBound);
    defstream.next_out = streamCompBuf.data();
    defstream.avail_out = static_cast<uInt>(streamOutBound);

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -6;
    }
    uLong streamCompLen = defstream.total_out;
    deflateEnd(&defstream);

    // Inflate (decompress) using z_stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -7;

    infstream.next_in = streamCompBuf.data();
    infstream.avail_in = static_cast<uInt>(streamCompLen);

    std::vector<Bytef> streamDecompBuf(origLen + 1);
    infstream.next_out = streamDecompBuf.data();
    infstream.avail_out = static_cast<uInt>(streamDecompBuf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -8;
    }
    uLong streamDecompLen = infstream.total_out;
    inflateEnd(&infstream);

    if (streamDecompLen != origLen) return -9;
    if (std::memcmp(streamDecompBuf.data(), original, origLen) != 0) return -10;

    // 3) gzip file API (gzopen/gzwrite/gzread)
    const char* gzFilename = "test_zlib_api_sequence_temp.gz";
    gzFile gz = gzopen(gzFilename, "wb");
    if (!gz) return -11;
    int written = gzwrite(gz, reinterpret_cast<const void*>(original), static_cast<unsigned int>(origLen));
    if (written != static_cast<int>(origLen)) {
        gzclose(gz);
        std::remove(gzFilename);
        return -12;
    }
    gzclose(gz);

    // Read back
    gz = gzopen(gzFilename, "rb");
    if (!gz) {
        std::remove(gzFilename);
        return -13;
    }
    std::vector<char> gzReadBuf(origLen + 1);
    int readBytes = gzread(gz, gzReadBuf.data(), static_cast<unsigned int>(origLen));
    gzclose(gz);
    // remove temp file
    std::remove(gzFilename);

    if (readBytes != static_cast<int>(origLen)) return -14;
    if (std::memcmp(gzReadBuf.data(), original, origLen) != 0) return -15;

    // If we reached here, all zlib API sequences succeeded.
    return 66;
}