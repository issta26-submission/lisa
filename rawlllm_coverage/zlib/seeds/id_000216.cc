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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include required headers here so the function is self-contained when pasted alone.
    // (Preprocessor directives are permitted anywhere in a translation unit.)
    #include <zlib.h>
    #include <vector>
    #include <cstring>

    // Original data to compress/decompress
    const char* original = "The quick brown fox jumps over the lazy dog";
    const size_t original_len = std::strlen(original);

    // 1) Simple one-shot compress / uncompress using compress() and uncompress()
    {
        uLong src_len = static_cast<uLong>(original_len);

        // allocate enough space for compressed data
        uLong comp_bound = compressBound(src_len);
        std::vector<unsigned char> compressed(comp_bound);

        int rc = compress(compressed.data(), &comp_bound,
                          reinterpret_cast<const Bytef*>(original), src_len);
        if (rc != Z_OK) return 1; // compress failed

        // now uncompress into a buffer of original size
        std::vector<unsigned char> uncompressed(src_len);
        uLong uncompressed_len = src_len;
        rc = uncompress(uncompressed.data(), &uncompressed_len,
                        compressed.data(), comp_bound);
        if (rc != Z_OK) return 2; // uncompress failed

        if (uncompressed_len != src_len) return 3;
        if (std::memcmp(uncompressed.data(), original, src_len) != 0) return 4;
    }

    // 2) Streaming deflate (deflateInit / deflate / deflateEnd) and inflate (inflateInit / inflate / inflateEnd)
    {
        // Prepare z_stream for deflate
        z_stream deflate_strm;
        std::memset(&deflate_strm, 0, sizeof(deflate_strm));
        int rc = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
        if (rc != Z_OK) return 10;

        // Output buffer for deflate: use compressBound to ensure it's large enough
        uLong src_len = static_cast<uLong>(original_len);
        uLong out_bound = compressBound(src_len);
        std::vector<unsigned char> deflated(out_bound);

        deflate_strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
        deflate_strm.avail_in = static_cast<uInt>(src_len);
        deflate_strm.next_out = deflated.data();
        deflate_strm.avail_out = static_cast<uInt>(deflated.size());

        // Perform deflate in one call with Z_FINISH (input fits in one shot)
        rc = deflate(&deflate_strm, Z_FINISH);
        if (rc != Z_STREAM_END) {
            deflateEnd(&deflate_strm);
            return 11;
        }
        size_t deflated_size = deflated.size() - deflate_strm.avail_out;

        // Clean up deflate
        deflateEnd(&deflate_strm);

        // Prepare z_stream for inflate
        z_stream inflate_strm;
        std::memset(&inflate_strm, 0, sizeof(inflate_strm));
        rc = inflateInit(&inflate_strm);
        if (rc != Z_OK) return 12;

        inflate_strm.next_in = deflated.data();
        inflate_strm.avail_in = static_cast<uInt>(deflated_size);

        // Output buffer for inflate: original size
        std::vector<unsigned char> inflated(original_len);
        inflate_strm.next_out = inflated.data();
        inflate_strm.avail_out = static_cast<uInt>(inflated.size());

        rc = inflate(&inflate_strm, Z_FINISH);
        if (rc != Z_STREAM_END) {
            inflateEnd(&inflate_strm);
            return 13;
        }
        size_t inflated_size = inflated.size() - inflate_strm.avail_out;

        // Clean up inflate
        inflateEnd(&inflate_strm);

        if (inflated_size != original_len) return 14;
        if (std::memcmp(inflated.data(), original, original_len) != 0) return 15;
    }

    // If all checks passed, return the required success value 66
    return 66;
}