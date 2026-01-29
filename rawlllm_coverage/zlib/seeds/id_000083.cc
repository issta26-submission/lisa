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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // The function demonstrates a correct usage sequence of zlib APIs:
    // - deflateInit / deflate / deflateEnd to compress data
    // - inflateInit / inflate / inflateEnd to decompress data
    // It verifies the round-trip integrity and returns 66 on success.
    //
    // Note: This function includes the necessary headers via preprocessor directives.
    // Including headers inside a function is valid (preprocessor runs before compilation),
    // and ensures this snippet compiles when pasted standalone.

    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    const std::string original = std::string(
        "The quick brown fox jumps over the lazy dog. "
        "This is a test of the zlib API sequence: deflate/inflate. "
        "Repeat to increase size. ") + std::string(16, 'X');

    const size_t CHUNK = 16384;

    // Compress with deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    std::vector<unsigned char> compressed;
    compressed.reserve(deflateBound(&defstream, static_cast<uLong>(original.size())));

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(original.size());

    unsigned char outbuf[CHUNK];
    for (;;) {
        defstream.next_out = outbuf;
        defstream.avail_out = CHUNK;
        ret = deflate(&defstream, Z_FINISH); // compress all input and finish
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -2;
        }
        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), outbuf, outbuf + have);
        if (ret == Z_STREAM_END) break;
        // otherwise continue looping until Z_STREAM_END
    }

    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return -3;
    }

    // Decompress with inflate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4;
    }

    std::vector<unsigned char> decompressed;
    decompressed.reserve(original.size());

    infstream.next_in = compressed.empty() ? Z_NULL : compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed.size());

    for (;;) {
        infstream.next_out = outbuf;
        infstream.avail_out = CHUNK;
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -5;
        }
        size_t have = CHUNK - infstream.avail_out;
        decompressed.insert(decompressed.end(), outbuf, outbuf + have);
        if (ret == Z_STREAM_END) break;
        // continue until stream end
    }

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return -6;
    }

    // Verify integrity
    if (decompressed.size() != original.size()) {
        return -7;
    }
    if (std::memcmp(decompressed.data(), original.data(), original.size()) != 0) {
        return -8;
    }

    // Optionally demonstrate another zlib utility: compute adler32 checksum
    // to show use of another part of the API (not required for correctness).
    uLong orig_adler = adler32(0L, Z_NULL, 0);
    orig_adler = adler32(orig_adler, reinterpret_cast<const Bytef*>(original.data()),
                         static_cast<uInt>(original.size()));
    uLong decomp_adler = adler32(0L, Z_NULL, 0);
    decomp_adler = adler32(decomp_adler, decompressed.data(),
                           static_cast<uInt>(decompressed.size()));
    if (orig_adler != decomp_adler) {
        return -9;
    }

    return 66;
}