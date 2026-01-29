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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a sequence of zlib API usages:
    // - compress / uncompress (single-call)
    // - deflateInit / deflate / deflateEnd (streamed compression)
    // - inflateInit / inflate / inflateEnd (streamed decompression)
    //
    // On success it returns 66. On failure it returns a non-66 value.

    // Include headers via preprocessor lines are allowed anywhere; place them here so
    // the function is self-contained. (In normal code these would be at file top.)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    const std::string original =
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string to exercise zlib compression and decompression APIs. "
        "Repeat to make it a bit larger: "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // 1) Single-call compress / uncompress
    {
        uLong sourceLen = static_cast<uLong>(original.size());
        uLong bound = compressBound(sourceLen);
        std::vector<Bytef> comp(bound);

        int comp_ret = compress(comp.data(), &bound,
                                reinterpret_cast<const Bytef*>(original.data()), sourceLen);
        if (comp_ret != Z_OK) return -1; // compress failed

        // Now uncompress
        std::vector<Bytef> uncmp(sourceLen); // we know the original size
        uLongf uncmpLen = sourceLen;
        int uncomp_ret = uncompress(uncmp.data(), &uncmpLen, comp.data(), bound);
        if (uncomp_ret != Z_OK) return -2; // uncompress failed
        if (uncmpLen != sourceLen) return -3;
        if (std::memcmp(uncmp.data(), original.data(), sourceLen) != 0) return -4;
    }

    // 2) Streamed compression with deflate / deflateEnd
    std::vector<Bytef> streamed_comp;
    {
        z_stream defstrm;
        std::memset(&defstrm, 0, sizeof(defstrm));
        int ret = deflateInit(&defstrm, Z_BEST_COMPRESSION);
        if (ret != Z_OK) return -5;

        const Bytef* in_ptr = reinterpret_cast<const Bytef*>(original.data());
        uInt in_left = static_cast<uInt>(original.size()); // safe for small test data
        defstrm.next_in = const_cast<Bytef*>(in_ptr);
        defstrm.avail_in = in_left;

        const size_t CHUNK = 128;
        std::vector<Bytef> outbuf(CHUNK);

        // We will call deflate with Z_FINISH because we supply the entire input.
        for (;;) {
            defstrm.next_out = outbuf.data();
            defstrm.avail_out = static_cast<uInt>(outbuf.size());

            ret = deflate(&defstrm, Z_FINISH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                deflateEnd(&defstrm);
                return -6; // deflate error
            }

            size_t have = outbuf.size() - defstrm.avail_out;
            if (have > 0) {
                streamed_comp.insert(streamed_comp.end(), outbuf.data(), outbuf.data() + have);
            }

            if (ret == Z_STREAM_END) break;
            // otherwise loop to continue draining output
        }

        deflateEnd(&defstrm);
    }

    // 3) Streamed decompression with inflate / inflateEnd
    std::vector<Bytef> streamed_decomp;
    {
        z_stream instrm;
        std::memset(&instrm, 0, sizeof(instrm));
        int ret = inflateInit(&instrm);
        if (ret != Z_OK) return -7;

        if (streamed_comp.empty()) {
            inflateEnd(&instrm);
            return -8; // nothing to inflate
        }

        instrm.next_in = streamed_comp.data();
        instrm.avail_in = static_cast<uInt>(streamed_comp.size());

        const size_t CHUNK = 128;
        std::vector<Bytef> outbuf(CHUNK);

        for (;;) {
            instrm.next_out = outbuf.data();
            instrm.avail_out = static_cast<uInt>(outbuf.size());

            ret = inflate(&instrm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                inflateEnd(&instrm);
                return -9; // inflate error
            }

            size_t have = outbuf.size() - instrm.avail_out;
            if (have > 0) {
                streamed_decomp.insert(streamed_decomp.end(), outbuf.data(), outbuf.data() + have);
            }

            if (ret == Z_STREAM_END) break;
            // continue if more output remaining
            if (instrm.avail_in == 0 && instrm.avail_out != 0) {
                // No more input and no need for more output => done
                break;
            }
        }

        inflateEnd(&instrm);
    }

    // Validate streamed decompressed output matches original
    if (streamed_decomp.size() != original.size()) return -10;
    if (std::memcmp(streamed_decomp.data(), original.data(), original.size()) != 0) return -11;

    // All checks passed
    return 66;
}