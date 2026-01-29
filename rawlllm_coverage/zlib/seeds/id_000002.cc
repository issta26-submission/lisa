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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when pasted into a file.
    // (Preprocessor includes are allowed inside function scope.)
    #include <zlib.h>
    #include <vector>
    #include <cstring>

    // Sample input data
    const char *input_str = "The quick brown fox jumps over the lazy dog. "
                            "This is a test string for zlib compression and decompression.";
    size_t input_len = std::strlen(input_str);

    // Prepare deflate (compression) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    bool deflate_inited = false;
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1; // failed to init deflate
    }
    deflate_inited = true;

    // Allocate output buffer sized with compressBound to ensure enough space
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> comp_buf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_str));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = comp_buf.data();
    defstream.avail_out = static_cast<uInt>(comp_buf.size());

    // Perform compression (single call is okay since buffer is big enough)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        if (deflate_inited) deflateEnd(&defstream);
        return -2; // compression failed
    }
    // compressed size:
    size_t compressed_size = comp_buf.size() - defstream.avail_out;

    // Clean up deflate
    if (deflate_inited) {
        deflateEnd(&defstream);
        deflate_inited = false;
    }

    // Prepare inflate (decompression) stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    bool inflate_inited = false;
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3; // failed to init inflate
    }
    inflate_inited = true;

    // Output buffer for decompressed data (we expect original size)
    std::vector<unsigned char> decomp_buf(input_len + 1); // +1 for safety / potential null-terminator

    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(decomp_buf.size());

    // Perform decompression
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        if (inflate_inited) inflateEnd(&infstream);
        return -4; // decompression failed
    }
    size_t decompressed_size = decomp_buf.size() - infstream.avail_out;

    // Clean up inflate
    if (inflate_inited) {
        inflateEnd(&infstream);
        inflate_inited = false;
    }

    // Validate that decompressed data matches the original
    if (decompressed_size != input_len) {
        return -5; // size mismatch
    }
    if (std::memcmp(decomp_buf.data(), input_str, input_len) != 0) {
        return -6; // content mismatch
    }

    // Success: return 66 as requested
    return 66;
}