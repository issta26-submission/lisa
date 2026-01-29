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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers here so the function is self-contained when copied into a single file
    #include <zlib.h>
    #include <string.h>
    #include <vector>

    // Sample input to compress and decompress
    const char* input = "The quick brown fox jumps over the lazy dog. zlib test string for compression/decompression.";
    const size_t input_len = strlen(input);

    // --- Compression using deflate API sequence ---
    z_stream defstream;
    memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len); // zlib uses uInt for avail_in

    // Allocate output buffer using compressBound to ensure it's large enough
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> compbuf(bound);
    defstream.next_out = compbuf.data();
    defstream.avail_out = static_cast<uInt>(compbuf.size());

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // failed to initialize deflate
        return -1;
    }

    // Run deflate until finished
    int deflate_ret;
    do {
        deflate_ret = deflate(&defstream, Z_FINISH);
        if (deflate_ret == Z_OK) {
            // output buffer might not be big enough in theory, but we sized it using compressBound
            // if this happens, treat as error
            deflateEnd(&defstream);
            return -2;
        }
    } while (deflate_ret == Z_OK);

    if (deflate_ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -3;
    }

    // Actual compressed size
    size_t comp_size = defstream.total_out;
    deflateEnd(&defstream);

    // --- Decompression using inflate API sequence ---
    z_stream infstream;
    memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.next_in = compbuf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    // Allocate a buffer for decompressed data. We know the original size.
    std::vector<unsigned char> decompbuf(input_len + 1); // +1 in case we want to null-terminate
    infstream.next_out = decompbuf.data();
    infstream.avail_out = static_cast<uInt>(decompbuf.size());

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4;
    }

    int inflate_ret;
    do {
        inflate_ret = inflate(&infstream, Z_FINISH);
        if (inflate_ret == Z_OK) {
            // should not normally happen because buffer should be large enough
            inflateEnd(&infstream);
            return -5;
        }
    } while (inflate_ret == Z_OK);

    if (inflate_ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -6;
    }

    size_t decomp_size = infstream.total_out;
    inflateEnd(&infstream);

    // Null-terminate for safe string comparison (we allocated +1)
    if (decomp_size < decompbuf.size()) {
        decompbuf[decomp_size] = 0;
    } else {
        // no room for null terminator (should not happen)
        return -7;
    }

    // Verify decompressed data matches original
    if (decomp_size != input_len) {
        return -8;
    }
    if (memcmp(decompbuf.data(), input, input_len) != 0) {
        return -9;
    }

    // If we've reached here, all zlib API usage succeeded and round-trip matched
    return 66;
}