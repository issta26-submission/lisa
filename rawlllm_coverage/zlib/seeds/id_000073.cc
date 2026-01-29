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
//<ID> 73
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Source data
    const char src[] =
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string to exercise zlib compression and decompression APIs.";
    uLong src_len = static_cast<uLong>(sizeof(src) - 1);

    // --- 1) Simple one-shot compress/uncompress using compress2/uncompress ---

    // allocate compress buffer using compressBound
    uLongf comp_bound = compressBound(src_len);
    Bytef* comp_buf = static_cast<Bytef*>(std::malloc(comp_bound));
    if (!comp_buf) return -1;

    uLongf comp_size = comp_bound;
    int ret = compress2(comp_buf, &comp_size, reinterpret_cast<const Bytef*>(src), src_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        std::free(comp_buf);
        return -2;
    }

    // allocate buffer to hold decompressed data (we know the original size)
    Bytef* decomp_buf = static_cast<Bytef*>(std::malloc(src_len));
    if (!decomp_buf) {
        std::free(comp_buf);
        return -3;
    }
    uLongf decomp_size = src_len;
    ret = uncompress(decomp_buf, &decomp_size, comp_buf, comp_size);
    if (ret != Z_OK) {
        std::free(comp_buf);
        std::free(decomp_buf);
        return -4;
    }

    // verify decompressed matches original
    if (decomp_size != src_len || std::memcmp(decomp_buf, src, src_len) != 0) {
        std::free(comp_buf);
        std::free(decomp_buf);
        return -5;
    }

    std::free(comp_buf);
    std::free(decomp_buf);

    // --- 2) Stream-based deflate/inflate (z_stream) ---

    // Prepare z_stream for deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -6;

    // Set input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstream.avail_in = static_cast<uInt>(src_len); // safe: src_len small

    // allocate output buffer for stream deflate
    uLongf stream_comp_bound = compressBound(src_len);
    Bytef* stream_comp_buf = static_cast<Bytef*>(std::malloc(stream_comp_bound));
    if (!stream_comp_buf) {
        deflateEnd(&defstream);
        return -7;
    }
    defstream.next_out = stream_comp_buf;
    defstream.avail_out = static_cast<uInt>(stream_comp_bound);

    // Perform deflate in one step
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // could be Z_OK if output buffer too small; treat as error for this simple test
        std::free(stream_comp_buf);
        deflateEnd(&defstream);
        return -8;
    }
    uLongf stream_comp_size = stream_comp_bound - defstream.avail_out;
    deflateEnd(&defstream);

    // Prepare z_stream for inflate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        std::free(stream_comp_buf);
        return -9;
    }

    infstream.next_in = stream_comp_buf;
    infstream.avail_in = static_cast<uInt>(stream_comp_size);

    // allocate buffer for inflated output
    Bytef* stream_decomp_buf = static_cast<Bytef*>(std::malloc(src_len));
    if (!stream_decomp_buf) {
        std::free(stream_comp_buf);
        inflateEnd(&infstream);
        return -10;
    }
    infstream.next_out = stream_decomp_buf;
    infstream.avail_out = static_cast<uInt>(src_len);

    // Perform inflate
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        std::free(stream_comp_buf);
        std::free(stream_decomp_buf);
        inflateEnd(&infstream);
        return -11;
    }
    uLongf stream_decomp_size = src_len - infstream.avail_out;
    inflateEnd(&infstream);

    // verify stream-decompressed data matches original
    if (stream_decomp_size != src_len || std::memcmp(stream_decomp_buf, src, src_len) != 0) {
        std::free(stream_comp_buf);
        std::free(stream_decomp_buf);
        return -12;
    }

    std::free(stream_comp_buf);
    std::free(stream_decomp_buf);

    // All tests passed; per request, return 66 on success
    return 66;
}