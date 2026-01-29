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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress and then decompress
    const std::string original = "The quick brown fox jumps over the lazy dog. zlib API sequence test.";
    const size_t original_len = original.size();
    const uLong src_len = static_cast<uLong>(original_len);

    // --- Compression (deflate) ---
    // Compute an upper bound for compressed data size
    uLong comp_bound = compressBound(src_len);
    std::vector<Bytef> comp_buf(comp_bound);

    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // failed to initialize deflate
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = src_len;
    defstream.next_out = comp_buf.data();
    defstream.avail_out = comp_bound;

    // Perform compression. Use Z_FINISH to indicate we're providing all input.
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // compression did not finish as expected
        deflateEnd(&defstream);
        return -2;
    }

    // Calculate actual compressed size
    uLong compressed_size = comp_bound - defstream.avail_out;

    // Clean up deflate state
    deflateEnd(&defstream);

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3;
    }

    infstream.next_in = comp_buf.data();
    infstream.avail_in = compressed_size;

    // Prepare output buffer; start with original size (most likely enough)
    std::vector<char> decomp_buf;
    decomp_buf.resize(original_len);
    infstream.next_out = reinterpret_cast<Bytef*>(decomp_buf.data());
    infstream.avail_out = static_cast<uInt>(decomp_buf.size());

    // Inflate in a loop to handle the possibility of needing more output space
    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break; // decompression finished successfully
        }
        if (ret != Z_OK) {
            // decompression error
            inflateEnd(&infstream);
            return -4;
        }

        // If output buffer filled but stream not finished, expand buffer and continue
        if (infstream.avail_out == 0) {
            size_t old_size = decomp_buf.size();
            decomp_buf.resize(old_size * 2);
            infstream.next_out = reinterpret_cast<Bytef*>(decomp_buf.data() + old_size);
            infstream.avail_out = static_cast<uInt>(old_size);
        }
    }

    // Get actual decompressed size
    uLong decompressed_size = infstream.total_out;

    // Clean up inflate state
    inflateEnd(&infstream);

    // Validate decompressed data matches original
    if (decompressed_size != src_len) {
        return -5;
    }
    if (std::memcmp(decomp_buf.data(), original.data(), original_len) != 0) {
        return -6;
    }

    // If we reach here, compression and decompression succeeded and data matches
    return 66;
}