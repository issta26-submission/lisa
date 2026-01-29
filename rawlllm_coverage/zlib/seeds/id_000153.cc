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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress
    const char original[] = "Hello zlib API sequence test";
    const std::size_t original_len = std::strlen(original) + 1; // include terminating NUL

    // --- Deflate (compress) ---
    z_stream defstrm{};
    bool deflate_initialized = false;
    int ret;

    // Initialize deflate stream
    defstrm.zalloc = Z_NULL;
    defstrm.zfree  = Z_NULL;
    defstrm.opaque = Z_NULL;
    ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }
    deflate_initialized = true;

    // Allocate output buffer using compressBound to guarantee enough space
    uLong bound = compressBound(static_cast<uLong>(original_len));
    std::vector<unsigned char> comp_buf(bound);
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    defstrm.avail_in = static_cast<uInt>(original_len);
    defstrm.next_out = comp_buf.data();
    defstrm.avail_out = static_cast<uInt>(comp_buf.size());

    // Perform compression with Z_FINISH (single call is safe because of compressBound)
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        if (deflate_initialized) deflateEnd(&defstrm);
        return -2;
    }
    // size of compressed data
    std::size_t compressed_size = comp_buf.size() - defstrm.avail_out;

    // Clean up deflate
    ret = deflateEnd(&defstrm);
    deflate_initialized = false;
    if (ret != Z_OK) {
        return -3;
    }

    // --- Inflate (decompress) ---
    z_stream infstrm{};
    bool inflate_initialized = false;
    infstrm.zalloc = Z_NULL;
    infstrm.zfree  = Z_NULL;
    infstrm.opaque = Z_NULL;

    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return -4;
    }
    inflate_initialized = true;

    // Prepare output buffer for decompression. We know original size, include some slack.
    std::vector<unsigned char> decomp_buf(original_len + 16);
    infstrm.next_in = comp_buf.data();
    infstrm.avail_in = static_cast<uInt>(compressed_size);
    infstrm.next_out = decomp_buf.data();
    infstrm.avail_out = static_cast<uInt>(decomp_buf.size());

    // Perform inflation. Since we provided enough output space, a single call should finish.
    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        if (inflate_initialized) inflateEnd(&infstrm);
        return -5;
    }

    // size of decompressed data
    std::size_t decompressed_size = decomp_buf.size() - infstrm.avail_out;

    // Clean up inflate
    ret = inflateEnd(&infstrm);
    inflate_initialized = false;
    if (ret != Z_OK) {
        return -6;
    }

    // Verify decompressed data matches the original
    if (decompressed_size != original_len) {
        return -7;
    }
    if (std::memcmp(decomp_buf.data(), original, original_len) != 0) {
        return -8;
    }

    // Success: return 66 as requested
    return 66;
}