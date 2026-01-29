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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include necessary headers (allowed anywhere in the translation unit).
    // Placing them here makes the function self-contained for the purposes of this example.
    #include <zlib.h>
    #include <vector>
    #include <cstring>

    // Sample input to compress/decompress
    const char input[] = "The quick brown fox jumps over the lazy dog";
    const std::size_t input_len = std::strlen(input);

    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    bool deflate_initialized = false;
    bool inflate_initialized = false;

    int ret_code = 1; // default failure return (non-66)

    // --- Compression (deflate) ---
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return ret_code;
    }
    deflate_initialized = true;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);

    // Allocate output buffer with worst-case upper bound
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<Bytef> comp_buf(bound);
    defstream.next_out = comp_buf.data();
    defstream.avail_out = static_cast<uInt>(bound);

    // Perform compression with a single deflate call (Z_FINISH) because we've allocated enough space
    int dret = deflate(&defstream, Z_FINISH);
    if (dret != Z_STREAM_END) {
        // cleanup
        deflateEnd(&defstream);
        return ret_code;
    }

    // size of compressed data
    uLong comp_size = bound - defstream.avail_out;

    // End deflate
    if (deflateEnd(&defstream) != Z_OK) {
        return ret_code;
    }
    deflate_initialized = false;

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));

    if (inflateInit(&infstream) != Z_OK) {
        return ret_code;
    }
    inflate_initialized = true;

    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    // We know the original size, so allocate exactly that size for output
    std::vector<Bytef> decomp_buf(input_len);
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(input_len);

    int iret = inflate(&infstream, Z_FINISH);
    if (iret != Z_STREAM_END) {
        // cleanup
        inflateEnd(&infstream);
        return ret_code;
    }

    uLong decomp_size = input_len - infstream.avail_out;

    // End inflate
    if (inflateEnd(&infstream) != Z_OK) {
        return ret_code;
    }
    inflate_initialized = false;

    // Verify decompressed data matches original
    if (decomp_size != input_len) {
        return ret_code;
    }
    if (std::memcmp(decomp_buf.data(), input, input_len) != 0) {
        return ret_code;
    }

    // If we reach here, compression and decompression succeeded and data matches
    return 66;
}