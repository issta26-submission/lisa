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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers required for zlib API usage and basic C functions.
    // Placing includes here keeps the function self-contained for the user's request.
    #include <zlib.h>
    #include <cstring>
    #include <vector>
    #include <string>
    #include <cstdio>
    #include <cstdlib>

    // Sample input data
    const char *sample = "The quick brown fox jumps over the lazy dog. zlib test sample.";
    uLong sampleLen = static_cast<uLong>(std::strlen(sample) + 1); // include terminating NUL

    // 1) Simple one-shot compress()/uncompress() usage
    uLong bound = compressBound(sampleLen);
    std::vector<Bytef> compBuf(bound);
    uLong compLen = bound;
    int cret = compress(compBuf.data(), &compLen, reinterpret_cast<const Bytef*>(sample), sampleLen);
    if (cret != Z_OK) {
        return 1; // compress() failed
    }

    std::vector<Bytef> decompBuf(sampleLen);
    uLong decompLen = sampleLen;
    int uret = uncompress(decompBuf.data(), &decompLen, compBuf.data(), compLen);
    if (uret != Z_OK) {
        return 2; // uncompress() failed
    }

    if (decompLen != sampleLen || std::memcmp(decompBuf.data(), sample, sampleLen) != 0) {
        return 3; // mismatch after uncompress
    }

    // 2) Streaming compression with deflateInit/deflate/deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return 4;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
    defstream.avail_in = static_cast<uInt>(sampleLen);

    std::vector<Bytef> streamCompBuf(bound); // reuse bound as an upper limit
    defstream.next_out = streamCompBuf.data();
    defstream.avail_out = static_cast<uInt>(streamCompBuf.size());

    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 5; // deflate did not finish as expected
    }
    uLong streamCompLen = static_cast<uLong>(defstream.total_out);
    deflateEnd(&defstream);

    // 3) Streaming decompression with inflateInit/inflate/inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 6;
    }

    infstream.next_in = streamCompBuf.data();
    infstream.avail_in = static_cast<uInt>(streamCompLen);

    std::vector<Bytef> streamDecompBuf(sampleLen);
    infstream.next_out = streamDecompBuf.data();
    infstream.avail_out = static_cast<uInt>(streamDecompBuf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 7; // inflate did not finish as expected
    }
    uLong streamDecompLen = static_cast<uLong>(infstream.total_out);
    inflateEnd(&infstream);

    if (streamDecompLen != sampleLen || std::memcmp(streamDecompBuf.data(), sample, sampleLen) != 0) {
        return 8; // mismatch after streaming inflate
    }

    // 4) gz* API: write compressed data to a temporary gz file and read it back
    const char *gzname = "test_zlib_api_sequence_temp.gz";
    gzFile gz = gzopen(gzname, "wb");
    if (!gz) {
        return 9;
    }
    int written = gzwrite(gz, sample, static_cast<unsigned int>(sampleLen));
    if (written <= 0) {
        gzclose(gz);
        return 10;
    }
    if (gzclose(gz) != Z_OK) {
        return 11;
    }

    gz = gzopen(gzname, "rb");
    if (!gz) {
        return 12;
    }
    std::vector<char> gzReadBuf(sampleLen);
    int readBytes = gzread(gz, gzReadBuf.data(), static_cast<unsigned int>(gzReadBuf.size()));
    if (readBytes <= 0) {
        gzclose(gz);
        return 13;
    }
    if (gzclose(gz) != Z_OK) {
        return 14;
    }

    // remove temporary file
    std::remove(gzname);

    if (static_cast<uLong>(readBytes) != sampleLen || std::memcmp(gzReadBuf.data(), sample, sampleLen) != 0) {
        return 15; // mismatch after gz read
    }

    // All tests passed
    return 66;
}