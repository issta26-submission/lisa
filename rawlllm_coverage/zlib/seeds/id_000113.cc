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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when copied into a translation unit.
    // Preprocessor directives are allowed inside a function.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Prepare sample data to compress.
    std::string input;
    for (int i = 0; i < 1000; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    uLong src_len = static_cast<uLong>(input.size());

    // 1) Simple compress/uncompress (one-shot API)
    uLong comp_bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(comp_bound);
    uLong comp_len = comp_bound;
    int zres = compress2(comp_buf.data(), &comp_len,
                         reinterpret_cast<const Bytef*>(input.data()), src_len,
                         Z_BEST_COMPRESSION);
    if (zres != Z_OK) return -1;

    std::vector<unsigned char> decomp_buf(src_len);
    uLong decomp_len = src_len;
    zres = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_len);
    if (zres != Z_OK) return -2;
    if (decomp_len != src_len) return -3;
    if (std::memcmp(decomp_buf.data(), input.data(), src_len) != 0) return -4;

    // 2) Stream API: deflate (compress) with z_stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    zres = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (zres != Z_OK) return -5;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = src_len;

    std::vector<unsigned char> stream_out;
    stream_out.resize(comp_bound); // start with compressBound size
    size_t out_pos = 0;
    defstream.next_out = stream_out.data();
    defstream.avail_out = static_cast<uInt>(stream_out.size());

    // Loop until deflate finishes
    for (;;) {
        zres = deflate(&defstream, Z_FINISH);
        out_pos = stream_out.size() - defstream.avail_out;
        if (zres == Z_STREAM_END) break;
        if (zres == Z_OK) {
            // Need more output space
            if (defstream.avail_out == 0) {
                size_t old_size = stream_out.size();
                size_t new_size = old_size * 2 + 1;
                stream_out.resize(new_size);
                defstream.next_out = stream_out.data() + out_pos;
                defstream.avail_out = static_cast<uInt>(new_size - out_pos);
                continue;
            } else {
                // Unexpected: deflate returned Z_OK but still has output room; continue loop.
                continue;
            }
        } else {
            deflateEnd(&defstream);
            return -6;
        }
    }

    // finalize deflate
    if (deflateEnd(&defstream) != Z_OK) return -7;
    size_t deflated_size = out_pos;
    stream_out.resize(deflated_size);

    // 3) Stream API: inflate (decompress) with z_stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    zres = inflateInit(&infstream);
    if (zres != Z_OK) return -8;

    infstream.next_in = stream_out.data();
    infstream.avail_in = static_cast<uInt>(deflated_size);

    std::vector<unsigned char> inflate_out;
    inflate_out.resize(src_len); // we expect original size
    size_t infl_out_pos = 0;
    infstream.next_out = inflate_out.data();
    infstream.avail_out = static_cast<uInt>(inflate_out.size());

    // Loop until inflate finishes
    for (;;) {
        zres = inflate(&infstream, Z_FINISH);
        infl_out_pos = inflate_out.size() - infstream.avail_out;
        if (zres == Z_STREAM_END) break;
        if (zres == Z_OK) {
            if (infstream.avail_out == 0) {
                // Need more output space (shouldn't be needed for this input, but handle anyway)
                size_t old_size = inflate_out.size();
                size_t new_size = old_size * 2 + 1;
                inflate_out.resize(new_size);
                infstream.next_out = inflate_out.data() + infl_out_pos;
                infstream.avail_out = static_cast<uInt>(new_size - infl_out_pos);
                continue;
            } else {
                continue;
            }
        } else {
            inflateEnd(&infstream);
            return -9;
        }
    }

    if (inflateEnd(&infstream) != Z_OK) return -10;
    size_t inflated_size = infl_out_pos;
    inflate_out.resize(inflated_size);

    if (inflated_size != src_len) return -11;
    if (std::memcmp(inflate_out.data(), input.data(), src_len) != 0) return -12;

    // All zlib API sequences succeeded and results match the original data.
    return 66;
}