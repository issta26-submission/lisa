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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct zlib API sequence:
    // - initialize a deflate (compression) stream
    // - compress a small buffer
    // - clean up the deflate stream
    // - initialize an inflate (decompression) stream
    // - decompress the compressed buffer
    // - verify the decompressed bytes equal the original input
    //
    // On success the function returns 66. On any failure it returns a non-66 value.

    // Example input to compress/decompress
    const char input[] = "The quick brown fox jumps over the lazy dog";
    const size_t input_size = strlen(input);

    // --- 1) Compress using deflate ---
    z_stream dstream{};
    dstream.zalloc = Z_NULL;
    dstream.zfree = Z_NULL;
    dstream.opaque = Z_NULL;

    int ret = deflateInit(&dstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // Allocate worst-case output buffer for compression
    uLong bound = compressBound(static_cast<uLong>(input_size));
    std::vector<unsigned char> comp_buf(bound);

    dstream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input));
    dstream.avail_in = static_cast<uInt>(input_size);
    dstream.next_out = comp_buf.data();
    dstream.avail_out = static_cast<uInt>(bound);

    // Perform compression with Z_FINISH. Using compressBound should allow finishing in one call.
    ret = deflate(&dstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up and return error
        deflateEnd(&dstream);
        return -2;
    }

    // total_out holds compressed size
    uLong comp_size = dstream.total_out;

    // End the deflate stream
    ret = deflateEnd(&dstream);
    if (ret != Z_OK) {
        return -3;
    }

    // --- 2) Decompress using inflate ---
    z_stream istream{};
    istream.zalloc = Z_NULL;
    istream.zfree = Z_NULL;
    istream.opaque = Z_NULL;

    ret = inflateInit(&istream);
    if (ret != Z_OK) {
        return -4;
    }

    std::vector<unsigned char> decomp_buf(input_size); // we expect original size after decompression

    istream.next_in = comp_buf.data();
    istream.avail_in = static_cast<uInt>(comp_size);
    istream.next_out = decomp_buf.data();
    istream.avail_out = static_cast<uInt>(input_size);

    // Perform decompression
    ret = inflate(&istream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istream);
        return -5;
    }

    uLong decomp_size = istream.total_out;

    // End the inflate stream
    ret = inflateEnd(&istream);
    if (ret != Z_OK) {
        return -6;
    }

    // --- 3) Verify ---
    if (decomp_size != input_size) {
        return -7;
    }
    if (memcmp(decomp_buf.data(), input, input_size) != 0) {
        return -8;
    }

    // If we reach here, all zlib calls succeeded and data round-tripped correctly.
    return 66;
}