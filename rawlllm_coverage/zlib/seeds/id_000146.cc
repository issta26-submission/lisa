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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a full compress -> decompress sequence using zlib APIs.
    // It returns 66 on success, other values indicate different failure points.

    const std::string original = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a short text repeated several times to make compression effective. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // Prepare deflate (compression) stream
    z_stream deflate_strm{};
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree  = Z_NULL;
    deflate_strm.opaque = Z_NULL;

    int ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1; // deflateInit failure
    }

    // Compute a safe upper bound for compressed data and allocate buffer
    uLong src_len = static_cast<uLong>(original.size());
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(bound);

    deflate_strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    deflate_strm.avail_in = static_cast<uInt>(src_len); // safe: input is small
    deflate_strm.next_out = comp_buf.data();
    deflate_strm.avail_out = static_cast<uInt>(bound);

    // Run deflate until stream ends
    while (true) {
        ret = deflate(&deflate_strm, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            deflateEnd(&deflate_strm);
            return -2; // deflate failure
        }
        // If returned Z_OK, but not finished, continue (shouldn't happen with compressBound buffer)
    }

    uLong comp_size = deflate_strm.total_out;
    // Clean up deflate
    deflateEnd(&deflate_strm);

    // Prepare inflate (decompression) stream
    z_stream inflate_strm{};
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree  = Z_NULL;
    inflate_strm.opaque = Z_NULL;

    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) {
        return -3; // inflateInit failure
    }

    std::vector<unsigned char> decomp_buf(src_len); // we expect original size after decompression

    inflate_strm.next_in = comp_buf.data();
    inflate_strm.avail_in = static_cast<uInt>(comp_size);
    inflate_strm.next_out = decomp_buf.data();
    inflate_strm.avail_out = static_cast<uInt>(src_len);

    // Run inflate until stream ends
    while (true) {
        ret = inflate(&inflate_strm, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            inflateEnd(&inflate_strm);
            return -4; // inflate failure
        }
        // If returned Z_OK and not finished, continue (shouldn't happen with correct buffer sizes)
    }

    uLong decomp_size = inflate_strm.total_out;
    inflateEnd(&inflate_strm);

    // Validate sizes match and content matches original
    if (decomp_size != src_len) {
        return -5; // size mismatch
    }

    std::string recovered(reinterpret_cast<char*>(decomp_buf.data()), static_cast<size_t>(decomp_size));
    if (recovered != original) {
        return -6; // content mismatch
    }

    // Success
    return 66;
}