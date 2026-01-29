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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when pasted into a file.
    // (Including inside a function is allowed by the preprocessor.)
    #include <zlib.h>
    #include <cstring>
    #include <vector>

    // Sample input data
    const char *input_str = "The quick brown fox jumps over the lazy dog. "
                            "This is a test of zlib API sequencing. "
                            "Repeat: The quick brown fox jumps over the lazy dog.";
    const std::size_t input_len = std::strlen(input_str);

    // 1) Use compressBound + compress (convenience API)
    uLongf comp1_len = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> comp1_buf(comp1_len);
    int comp_ret = compress(comp1_buf.data(), &comp1_len,
                            reinterpret_cast<const Bytef*>(input_str),
                            static_cast<uLong>(input_len));
    if (comp_ret != Z_OK) {
        return -1;
    }
    comp1_buf.resize(comp1_len);

    // 2) Use uncompress (convenience API) to get back original data
    std::vector<unsigned char> uncmp1_buf(input_len);
    uLongf uncmp1_len = static_cast<uLong>(input_len);
    int uncmp_ret = uncompress(uncmp1_buf.data(), &uncmp1_len,
                               comp1_buf.data(), comp1_len);
    if (uncmp_ret != Z_OK || uncmp1_len != input_len) {
        return -2;
    }
    if (std::memcmp(uncmp1_buf.data(), input_str, input_len) != 0) {
        return -3;
    }

    // 3) Use low-level deflate streaming API (deflateInit, deflate, deflateEnd)
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    int dinit_ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (dinit_ret != Z_OK) {
        return -4;
    }

    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_str));
    // zlib avail_in is uInt; our input is small so this cast is safe
    dstrm.avail_in = static_cast<uInt>(input_len);

    std::vector<unsigned char> def_out_buf;
    const std::size_t out_chunk = 128;
    std::vector<unsigned char> tmp_out(out_chunk);

    // Call deflate with Z_FINISH because we provide all input at once
    int def_ret;
    do {
        dstrm.next_out = tmp_out.data();
        dstrm.avail_out = static_cast<uInt>(tmp_out.size());
        def_ret = deflate(&dstrm, Z_FINISH);
        if (def_ret != Z_OK && def_ret != Z_STREAM_END) {
            deflateEnd(&dstrm);
            return -5;
        }
        std::size_t produced = tmp_out.size() - dstrm.avail_out;
        def_out_buf.insert(def_out_buf.end(), tmp_out.data(), tmp_out.data() + produced);
    } while (def_ret != Z_STREAM_END);

    int dend_ret = deflateEnd(&dstrm);
    if (dend_ret != Z_OK) {
        return -6;
    }

    // 4) Use inflate streaming API (inflateInit, inflate, inflateEnd)
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    int iinit_ret = inflateInit(&istrm);
    if (iinit_ret != Z_OK) {
        return -7;
    }

    istrm.next_in = def_out_buf.empty() ? Z_NULL : def_out_buf.data();
    istrm.avail_in = static_cast<uInt>(def_out_buf.size());

    std::vector<unsigned char> infl_out_buf;
    std::vector<unsigned char> tmp_infl(out_chunk);
    int infl_ret;
    do {
        istrm.next_out = tmp_infl.data();
        istrm.avail_out = static_cast<uInt>(tmp_infl.size());
        infl_ret = inflate(&istrm, Z_NO_FLUSH);
        if (infl_ret != Z_OK && infl_ret != Z_STREAM_END) {
            inflateEnd(&istrm);
            return -8;
        }
        std::size_t out_produced = tmp_infl.size() - istrm.avail_out;
        infl_out_buf.insert(infl_out_buf.end(), tmp_infl.data(), tmp_infl.data() + out_produced);
    } while (infl_ret != Z_STREAM_END);

    int iend_ret = inflateEnd(&istrm);
    if (iend_ret != Z_OK) {
        return -9;
    }

    // Verify inflate result matches original
    if (infl_out_buf.size() != input_len) {
        return -10;
    }
    if (std::memcmp(infl_out_buf.data(), input_str, input_len) != 0) {
        return -11;
    }

    // 5) Use crc32 on original and on the inflated output to verify checksum equality
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(input_str),
                     static_cast<uInt>(input_len));
    uLong crc_infl = crc32(0L, Z_NULL, 0);
    crc_infl = crc32(crc_infl, infl_out_buf.data(), static_cast<uInt>(infl_out_buf.size()));
    if (crc_orig != crc_infl) {
        return -12;
    }

    // If all steps succeeded, return 66 as requested.
    return 66;
}