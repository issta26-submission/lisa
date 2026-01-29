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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a test of the zlib API sequence. "
                              "Repeat: The quick brown fox jumps over the lazy dog. "
                              "End of message.";

    const Bytef* in_data = reinterpret_cast<const Bytef*>(input.data());
    uInt in_size = static_cast<uInt>(input.size());

    // Compute CRC32 of original data
    uLong orig_crc = crc32(0L, Z_NULL, 0);
    orig_crc = crc32(orig_crc, in_data, in_size);

    // --- Compress using deflate (stream API) ---
    z_stream deflate_strm;
    std::memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;

    int ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1;
    }

    // Allocate output buffer using compressBound to ensure it's large enough
    uLong bound = compressBound(in_size);
    std::vector<Bytef> comp_buf(bound);
    deflate_strm.next_in = const_cast<Bytef*>(in_data);
    deflate_strm.avail_in = in_size;
    deflate_strm.next_out = comp_buf.data();
    deflate_strm.avail_out = static_cast<uInt>(comp_buf.size());

    // Finish compression
    ret = deflate(&deflate_strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&deflate_strm);
        return 2;
    }

    // If not ended yet, continue until finished (should not be necessary with compressBound)
    while (ret == Z_OK) {
        // No more output space should be needed because we used compressBound,
        // but handle defensively by failing.
        deflateEnd(&deflate_strm);
        return 3;
    }

    // Total compressed size
    uLong comp_size = deflate_strm.total_out;
    deflateEnd(&deflate_strm);

    // --- Decompress using inflate (stream API) ---
    z_stream inflate_strm;
    std::memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;

    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) {
        return 4;
    }

    std::vector<Bytef> decomp_buf(in_size); // we expect original size
    inflate_strm.next_in = comp_buf.data();
    inflate_strm.avail_in = static_cast<uInt>(comp_size);
    inflate_strm.next_out = decomp_buf.data();
    inflate_strm.avail_out = static_cast<uInt>(decomp_buf.size());

    // Inflate until stream end
    ret = inflate(&inflate_strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&inflate_strm);
        return 5;
    }

    // If inflate returned Z_OK here (unlikely with correct sizes), continue
    while (ret == Z_OK) {
        // No extra output buffer handling needed because we sized for original data.
        inflateEnd(&inflate_strm);
        return 6;
    }

    uLong decomp_size = inflate_strm.total_out;
    inflateEnd(&inflate_strm);

    if (decomp_size != in_size) {
        return 7;
    }

    // Verify decompressed data matches original
    if (std::memcmp(decomp_buf.data(), in_data, in_size) != 0) {
        return 8;
    }

    // Verify CRC32 matches
    uLong decomp_crc = crc32(0L, Z_NULL, 0);
    decomp_crc = crc32(decomp_crc, decomp_buf.data(), static_cast<uInt>(decomp_size));
    if (decomp_crc != orig_crc) {
        return 9;
    }

    // All checks passed
    return 66;
}