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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data (can be any bytes)
    const std::string input = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test of the zlib API sequence using deflate/inflate and crc32 checks.";

    // Prepare deflate (compression) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // Initialize deflate
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1; // deflateInit failed
    }
    bool deflate_initialized = true;

    // Allocate output buffer using compressBound to ensure enough space
    uLong src_len = static_cast<uLong>(input.size());
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(src_len);
    defstream.next_out = comp_buf.data();
    defstream.avail_out = static_cast<uInt>(bound);

    // Perform compression in one step (buffer is large enough)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2; // deflate did not finish as expected
    }

    // Calculate compressed size
    size_t comp_size = bound - defstream.avail_out;

    // Clean up deflate
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return -3; // deflateEnd failed
    }
    deflate_initialized = false;

    // Prepare inflate (decompression) stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4; // inflateInit failed
    }
    bool inflate_initialized = true;

    // Prepare output buffer for decompressed data (we know original size)
    std::vector<unsigned char> decomp_buf(src_len);
    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(src_len);

    // Perform inflation
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5; // inflate did not finish as expected
    }

    // Calculate decompressed size
    size_t decomp_size = src_len - infstream.avail_out;

    // Clean up inflate
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return -6; // inflateEnd failed
    }
    inflate_initialized = false;

    // Verify sizes match original
    if (decomp_size != src_len) {
        return -7; // decompressed size mismatch
    }

    // Verify data integrity by comparing bytes and by CRC32
    if (std::memcmp(decomp_buf.data(), input.data(), src_len) != 0) {
        return -8; // decompressed content differs
    }

    // Compute CRC32 of original and decompressed to demonstrate use of crc32 API
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig,
                     reinterpret_cast<const Bytef*>(input.data()),
                     static_cast<uInt>(src_len));

    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp,
                       decomp_buf.data(),
                       static_cast<uInt>(decomp_size));

    if (crc_orig != crc_decomp) {
        return -9; // CRC mismatch (shouldn't happen if bytes matched)
    }

    // All checks passed
    return 66;
}