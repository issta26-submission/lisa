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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data
    const char* input = "Hello zlib! This is a test of multiple zlib APIs: compress/uncompress, "
                        "deflate/inflate (streaming) and checksum functions (crc32, adler32).";
    uLong input_len = static_cast<uLong>(strlen(input));

    // 1) Compute initial checksums
    uLong crc_initial = crc32(0L, Z_NULL, 0);
    crc_initial = crc32(crc_initial, reinterpret_cast<const Bytef*>(input), input_len);

    uLong adler_initial = adler32(0L, Z_NULL, 0);
    adler_initial = adler32(adler_initial, reinterpret_cast<const Bytef*>(input), input_len);

    // 2) Simple one-shot compression using compress2()
    uLongf comp_bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(comp_bound);
    int ret = compress2(comp_buf.data(), &comp_bound,
                        reinterpret_cast<const Bytef*>(input), input_len,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1;
    comp_buf.resize(comp_bound);

    // 3) One-shot decompression using uncompress()
    std::vector<Bytef> decomp_buf(input_len);
    uLongf decomp_len = input_len;
    ret = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_bound);
    if (ret != Z_OK) return -2;
    if (decomp_len != input_len) return -3;
    if (memcmp(decomp_buf.data(), input, input_len) != 0) return -4;

    // 4) Streaming compression (deflate) using z_stream
    z_stream deflate_strm;
    std::memset(&deflate_strm, 0, sizeof(deflate_strm));
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -5;

    deflate_strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    deflate_strm.avail_in = input_len;

    // allocate an output buffer (start with compressBound, grow if needed)
    std::vector<Bytef> stream_comp(comp_bound ? static_cast<size_t>(comp_bound) : 128);
    size_t out_pos = 0;

    while (true) {
        if (out_pos >= stream_comp.size()) {
            // expand output buffer
            stream_comp.resize(stream_comp.size() * 2);
        }
        deflate_strm.next_out = stream_comp.data() + out_pos;
        deflate_strm.avail_out = static_cast<uInt>(stream_comp.size() - out_pos);

        ret = deflate(&deflate_strm, Z_FINISH);
        out_pos = static_cast<size_t>(deflate_strm.total_out);

        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) { deflateEnd(&deflate_strm); return -6; }

        // need more output space, loop will expand buffer
    }
    deflateEnd(&deflate_strm);
    stream_comp.resize(out_pos);

    // 5) Streaming decompression (inflate) using z_stream
    z_stream inflate_strm;
    std::memset(&inflate_strm, 0, sizeof(inflate_strm));
    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) return -7;

    inflate_strm.next_in = stream_comp.data();
    inflate_strm.avail_in = static_cast<uInt>(stream_comp.size());

    std::vector<Bytef> stream_decomp(input_len);
    out_pos = 0;

    while (true) {
        if (out_pos >= stream_decomp.size()) {
            stream_decomp.resize(stream_decomp.size() * 2);
        }
        inflate_strm.next_out = stream_decomp.data() + out_pos;
        inflate_strm.avail_out = static_cast<uInt>(stream_decomp.size() - out_pos);

        ret = inflate(&inflate_strm, Z_FINISH);
        out_pos = static_cast<size_t>(inflate_strm.total_out);

        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) { inflateEnd(&inflate_strm); return -8; }

        // need more output space, loop will expand buffer
    }
    inflateEnd(&inflate_strm);
    stream_decomp.resize(out_pos);

    if (stream_decomp.size() != static_cast<size_t>(input_len)) return -9;
    if (memcmp(stream_decomp.data(), input, input_len) != 0) return -10;

    // 6) Recompute checksums on the decompressed (stream) data and compare
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after, stream_decomp.data(), static_cast<uInt>(stream_decomp.size()));
    if (crc_after != crc_initial) return -11;

    uLong adler_after = adler32(0L, Z_NULL, 0);
    adler_after = adler32(adler_after, stream_decomp.data(), static_cast<uInt>(stream_decomp.size()));
    if (adler_after != adler_initial) return -12;

    // All zlib API sequence checks passed
    return 66;
}