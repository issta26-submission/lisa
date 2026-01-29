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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Test function demonstrating several zlib API sequences.
// Returns 66 on success, other values indicate specific failures.
int test_zlib_api_sequence() {
    // Prepare input data
    const std::string text =
        "The quick brown fox jumps over the lazy dog. "
        "Zlib API sequence test. Repeat to increase size. ";
    std::string input_str;
    for (int i = 0; i < 16; ++i) input_str += text; // make input reasonably sized

    // Convert to a mutable byte buffer (zlib uses Bytef *)
    std::vector<Bytef> input_buf(input_str.begin(), input_str.end());
    uLong source_len = static_cast<uLong>(input_buf.size());
    if (source_len == 0) return 1;

    // 1) Simple one-shot compression/uncompression with compress2/uncompress
    uLongf comp_bound = compressBound(source_len);
    std::vector<Bytef> comp_buf(comp_bound);

    int rc = compress2(comp_buf.data(), &comp_bound,
                       input_buf.data(), source_len, Z_BEST_COMPRESSION);
    if (rc != Z_OK) return 2; // compression failed

    // Decompress back
    std::vector<Bytef> decomp_buf(source_len);
    uLongf decomp_len = source_len;
    rc = uncompress(decomp_buf.data(), &decomp_len, comp_buf.data(), comp_bound);
    if (rc != Z_OK) return 3; // uncompress failed
    if (decomp_len != source_len) return 4;
    if (std::memcmp(decomp_buf.data(), input_buf.data(), source_len) != 0) return 5;

    // 2) Streamed compression using deflateInit / deflate / deflateEnd
    z_stream dstream;
    std::memset(&dstream, 0, sizeof(dstream));
    dstream.zalloc = Z_NULL;
    dstream.zfree = Z_NULL;
    dstream.opaque = Z_NULL;

    rc = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (rc != Z_OK) return 6;

    const size_t CHUNK = 256;
    std::vector<Bytef> out_chunk(CHUNK);
    std::vector<Bytef> streamed_comp; // will collect all compressed output

    // feed input in one go (could be chunked); deflate with Z_NO_FLUSH until input consumed
    dstream.next_in = input_buf.data();
    dstream.avail_in = static_cast<uInt>(source_len);

    while (dstream.avail_in > 0) {
        dstream.next_out = out_chunk.data();
        dstream.avail_out = static_cast<uInt>(CHUNK);
        rc = deflate(&dstream, Z_NO_FLUSH);
        if (rc != Z_OK) { deflateEnd(&dstream); return 7; }
        size_t have = CHUNK - dstream.avail_out;
        streamed_comp.insert(streamed_comp.end(), out_chunk.data(), out_chunk.data() + have);
    }

    // finish the stream
    int deflate_ret;
    do {
        dstream.next_out = out_chunk.data();
        dstream.avail_out = static_cast<uInt>(CHUNK);
        deflate_ret = deflate(&dstream, Z_FINISH);
        if (deflate_ret != Z_OK && deflate_ret != Z_STREAM_END) { deflateEnd(&dstream); return 8; }
        size_t have = CHUNK - dstream.avail_out;
        streamed_comp.insert(streamed_comp.end(), out_chunk.data(), out_chunk.data() + have);
    } while (deflate_ret != Z_STREAM_END);

    deflateEnd(&dstream);

    // 3) Streamed decompression using inflateInit / inflate / inflateEnd
    z_stream istream;
    std::memset(&istream, 0, sizeof(istream));
    istream.zalloc = Z_NULL;
    istream.zfree = Z_NULL;
    istream.opaque = Z_NULL;

    rc = inflateInit(&istream);
    if (rc != Z_OK) return 9;

    std::vector<Bytef> streamed_decomp;
    istream.next_in = streamed_comp.empty() ? Z_NULL : streamed_comp.data();
    istream.avail_in = static_cast<uInt>(streamed_comp.size());

    int inflate_ret;
    do {
        istream.next_out = out_chunk.data();
        istream.avail_out = static_cast<uInt>(CHUNK);
        inflate_ret = inflate(&istream, Z_NO_FLUSH);
        if (inflate_ret != Z_OK && inflate_ret != Z_STREAM_END) { inflateEnd(&istream); return 10; }
        size_t have = CHUNK - istream.avail_out;
        streamed_decomp.insert(streamed_decomp.end(), out_chunk.data(), out_chunk.data() + have);
    } while (inflate_ret != Z_STREAM_END);

    inflateEnd(&istream);

    // Verify streamed decompressed data matches original
    if (streamed_decomp.size() != input_buf.size()) return 11;
    if (std::memcmp(streamed_decomp.data(), input_buf.data(), input_buf.size()) != 0) return 12;

    // If we reached here, all sequences succeeded
    return 66;
}