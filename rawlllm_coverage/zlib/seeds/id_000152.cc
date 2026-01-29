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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare input data
    const std::string input =
        "The quick brown fox jumps over the lazy dog. "
        "This is a sample text to demonstrate zlib compression and decompression. ";
    // Repeat it a few times to make it compressible
    std::string orig;
    for (int i = 0; i < 16; ++i) orig += input;
    const uLong orig_len = static_cast<uLong>(orig.size());

    // --- High-level API: compress2 + uncompress ---

    // Get an upper bound on compressed size
    uLong bound = compressBound(orig_len);
    std::vector<Bytef> comp_buf(bound);

    uLong comp_len = bound;
    int ret = compress2(comp_buf.data(), &comp_len,
                        reinterpret_cast<const Bytef*>(orig.data()), orig_len,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        // compression failed
        return 1;
    }

    // Decompress using uncompress
    std::vector<Bytef> decomp_buf(orig_len);
    uLong decomp_len = orig_len;
    ret = uncompress(decomp_buf.data(), &decomp_len,
                     comp_buf.data(), comp_len);
    if (ret != Z_OK || decomp_len != orig_len) {
        return 2;
    }

    if (std::memcmp(decomp_buf.data(), orig.data(), orig_len) != 0) {
        return 3; // decompressed data mismatch
    }

    // --- Low-level streaming API: deflate (stream) + inflate (stream) ---

    // Prepare z_stream for deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // Initialize deflate with default compression level
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 4;
    }

    // Prepare output buffer large enough (use compressBound as upper bound)
    std::vector<Bytef> stream_comp_buf(compressBound(orig_len));
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(orig.data()));
    defstream.avail_in = orig_len;
    defstream.next_out = stream_comp_buf.data();
    defstream.avail_out = static_cast<uInt>(stream_comp_buf.size());

    // Finish the stream in one call (since all input is provided)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&defstream);
        return 5;
    }
    // If returned Z_OK, call until Z_STREAM_END (loop to be robust)
    while (ret == Z_OK) {
        ret = deflate(&defstream, Z_FINISH);
    }
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 6;
    }

    const uLong stream_comp_len = stream_comp_buf.size() - defstream.avail_out;
    deflateEnd(&defstream);

    // Inflate the stream compressed data
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 7;
    }

    std::vector<Bytef> stream_decomp_buf(orig_len);
    infstream.next_in = stream_comp_buf.data();
    infstream.avail_in = static_cast<uInt>(stream_comp_len);
    infstream.next_out = stream_decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(stream_decomp_buf.size());

    // Inflate until stream end
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&infstream);
        return 8;
    }
    while (ret == Z_OK) {
        ret = inflate(&infstream, Z_FINISH);
    }
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 9;
    }

    const uLong stream_decomp_len = stream_decomp_buf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    if (stream_decomp_len != orig_len) {
        return 10;
    }
    if (std::memcmp(stream_decomp_buf.data(), orig.data(), orig_len) != 0) {
        return 11;
    }

    // If we reached here, both high-level and streaming sequences succeeded
    return 66;
}