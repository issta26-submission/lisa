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
//<ID> 162
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a test of zlib APIs. "
                              "Repeat to increase size. "
                              "The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog.";

    // 1) Compute CRC32 of original data
    uLong crc_before = crc32(0L, Z_NULL, 0);
    crc_before = crc32(crc_before,
                       reinterpret_cast<const Bytef*>(input.data()),
                       static_cast<uInt>(input.size()));

    // 2) Simple compress() / uncompress() round-trip
    uLongf comp_bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<Bytef> comp_buf(comp_bound);
    uLongf comp_size = comp_bound;
    int ret = compress(comp_buf.data(),
                       &comp_size,
                       reinterpret_cast<const Bytef*>(input.data()),
                       static_cast<uLong>(input.size()));
    if (ret != Z_OK) return 1;
    comp_buf.resize(comp_size);

    std::vector<Bytef> decomp_buf(input.size());
    uLongf decomp_size = static_cast<uLongf>(decomp_buf.size());
    ret = uncompress(decomp_buf.data(),
                     &decomp_size,
                     comp_buf.data(),
                     comp_buf.size());
    if (ret != Z_OK) return 2;
    if (decomp_size != input.size()) return 3;
    if (std::memcmp(decomp_buf.data(), input.data(), input.size()) != 0) return 4;

    // Verify CRC after uncompress
    uLong crc_after = crc32(0L, Z_NULL, 0);
    crc_after = crc32(crc_after,
                      decomp_buf.data(),
                      static_cast<uInt>(decomp_size));
    if (crc_before != crc_after) return 5;

    // 3) Streaming compression using deflate() API
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 6;

    defstream.next_in = const_cast<Bytef*>(
        reinterpret_cast<const Bytef*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    const size_t CHUNK = 128;
    std::vector<Bytef> out_chunk(CHUNK);
    std::vector<Bytef> deflated_stream;

    // Use Z_FINISH to finish the stream in this example
    int deflate_ret = Z_OK;
    while (deflate_ret != Z_STREAM_END) {
        defstream.next_out = out_chunk.data();
        defstream.avail_out = static_cast<uInt>(out_chunk.size());
        deflate_ret = deflate(&defstream, Z_FINISH);
        if (deflate_ret == Z_STREAM_ERROR) { deflateEnd(&defstream); return 7; }
        size_t have = out_chunk.size() - defstream.avail_out;
        deflated_stream.insert(deflated_stream.end(), out_chunk.data(), out_chunk.data() + have);
        // If avail_out != 0 and deflate_ret == Z_OK, loop will continue; if Z_STREAM_END break.
    }
    deflateEnd(&defstream);
    if (deflate_ret != Z_STREAM_END) return 8;

    // 4) Streaming decompression using inflate() API
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return 9;

    infstream.next_in = deflated_stream.empty() ? Z_NULL : deflated_stream.data();
    infstream.avail_in = static_cast<uInt>(deflated_stream.size());

    std::vector<Bytef> inflated_result;
    int inflate_ret = Z_OK;
    while (inflate_ret != Z_STREAM_END) {
        out_chunk.assign(CHUNK, 0);
        infstream.next_out = out_chunk.data();
        infstream.avail_out = static_cast<uInt>(out_chunk.size());
        inflate_ret = inflate(&infstream, Z_NO_FLUSH);
        if (inflate_ret == Z_NEED_DICT || inflate_ret == Z_DATA_ERROR || inflate_ret == Z_MEM_ERROR) {
            inflateEnd(&infstream);
            return 10;
        }
        size_t have = out_chunk.size() - infstream.avail_out;
        inflated_result.insert(inflated_result.end(), out_chunk.data(), out_chunk.data() + have);
        if (inflate_ret == Z_BUF_ERROR && infstream.avail_in == 0) {
            // No more input; break to avoid infinite loop
            break;
        }
    }
    inflateEnd(&infstream);

    if (inflated_result.size() != input.size()) return 11;
    if (std::memcmp(inflated_result.data(), input.data(), input.size()) != 0) return 12;

    // All checks passed; return 66 as requested
    return 66;
}