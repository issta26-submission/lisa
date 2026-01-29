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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare input data (repeated to make compression meaningful)
    const char *chunk = "The quick brown fox jumps over the lazy dog. ";
    std::string src;
    for (int i = 0; i < 200; ++i) src += chunk; // make it reasonably large
    size_t src_len = src.size();

    // --- Compression using deflate (z_stream) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // failed to initialize
        return 1;
    }

    // Allocate output buffer using compressBound which is guaranteed to be large enough
    uLong bound = compressBound(static_cast<uLong>(src_len));
    std::vector<unsigned char> compressed;
    compressed.resize(static_cast<size_t>(bound));

    defstream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(src.data()));
    defstream.avail_in = static_cast<uInt>(src_len);
    defstream.next_out = compressed.data();
    defstream.avail_out = static_cast<uInt>(compressed.size());

    // Perform compression; compressBound should be sufficient so one call with Z_FINISH ends the stream
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Something unexpected happened; clean up and fail
        deflateEnd(&defstream);
        return 2;
    }
    // Number of compressed bytes
    size_t compressed_size = static_cast<size_t>(defstream.total_out);

    // Clean up deflate state
    deflateEnd(&defstream);

    // Shrink vector to actual compressed size
    compressed.resize(compressed_size);

    // --- Decompression using inflate (z_stream) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 3;
    }

    // Prepare output buffer at least as large as original input
    std::vector<unsigned char> decompressed;
    decompressed.resize(src_len);

    infstream.next_in = compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);
    infstream.next_out = decompressed.data();
    infstream.avail_out = static_cast<uInt>(decompressed.size());

    // Inflate. Since we know the original size, one call with Z_FINISH should finish the stream.
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // If the destination buffer was too small, handle that (rare here because we sized it correctly)
        if (ret == Z_OK || ret == Z_BUF_ERROR) {
            // attempt to expand the output buffer and try again (robust fallback)
            size_t current_out = static_cast<size_t>(infstream.total_out);
            size_t needed = src_len; // we know original size; expand to that
            if (decompressed.size() < needed) decompressed.resize(needed);
            infstream.next_out = decompressed.data() + current_out;
            infstream.avail_out = static_cast<uInt>(decompressed.size() - current_out);
            ret = inflate(&infstream, Z_FINISH);
        }
    }

    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 4;
    }

    size_t decompressed_size = static_cast<size_t>(infstream.total_out);
    inflateEnd(&infstream);

    // Verify decompressed data matches original
    if (decompressed_size != src_len) {
        return 5;
    }
    if (std::memcmp(decompressed.data(), src.data(), src_len) != 0) {
        return 6;
    }

    // If everything succeeded, return 66 as requested
    return 66;
}