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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct sequence of zlib API usage:
    // - deflateInit / deflate / deflateEnd to compress
    // - inflateInit / inflate / inflateEnd to decompress
    // - compressBound and crc32 usage for validation
    //
    // Note: The translation unit including this function must #include <zlib.h>,
    // and also include the standard headers used below. If you place this in a
    // file, ensure you have:
    //   #include <zlib.h>
    //   #include <vector>
    //   #include <cstring>
    //   #include <string>
    // The function itself assumes those are visible to the compiler.

    // Sample input data to compress
    const char sample[] =
        "The quick brown fox jumps over the lazy dog. "
        "Zlib API sequence test for compression and decompression.";
    const std::size_t orig_len = std::strlen(sample);

    // Basic safety checks
    if (orig_len == 0) {
        // Nothing to test, but consider this success for the test
        return 66;
    }

    // Calculate upper bound for compressed size
    uLong bound = compressBound(static_cast<uLong>(orig_len));
    std::vector<unsigned char> compBuf(bound);

    // Initialize deflate (compress)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(sample));
    defstream.avail_in = static_cast<uInt>(orig_len);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    // Perform compression with single call since buffer is large enough (compressBound)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up and fail if not finished
        deflateEnd(&defstream);
        return -2;
    }

    // Determine compressed size
    std::size_t comp_size = compBuf.size() - defstream.avail_out;

    // Clean up deflate state
    if (deflateEnd(&defstream) != Z_OK) {
        return -3;
    }

    // Prepare buffer for decompression, use original size as expected output
    std::vector<unsigned char> decompBuf(orig_len + 1); // +1 for safety / potential null terminator
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4;
    }

    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    // Perform inflation
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Try to handle case where Z_OK is returned (shouldn't for full buffer),
        // but treat as error for this test.
        inflateEnd(&infstream);
        return -5;
    }

    std::size_t decomp_size = decompBuf.size() - infstream.avail_out;

    if (inflateEnd(&infstream) != Z_OK) {
        return -6;
    }

    // Validate decompressed size matches original and contents match
    if (decomp_size != orig_len) {
        return -7;
    }

    if (std::memcmp(decompBuf.data(), sample, orig_len) != 0) {
        return -8;
    }

    // Optional: verify CRC32 of original and decompressed data match
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(sample), static_cast<uInt>(orig_len));
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decompBuf.data(), static_cast<uInt>(decomp_size));
    if (crc_orig != crc_decomp) {
        return -9;
    }

    // If all steps succeeded, return the required 66
    return 66;
}