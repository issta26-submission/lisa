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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function remains self-contained when pasted into a source file.
    // It's valid for preprocessor includes to appear inside a function.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Prepare some sample data that compresses well.
    std::string input;
    for (int i = 0; i < 200; ++i) input += "The quick brown fox jumps over the lazy dog. ";

    int ret = Z_OK;
    bool deflate_initialized = false;
    bool inflate_initialized = false;

    // --- Compression (deflate) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 2; // deflateInit failed
    }
    deflate_initialized = true;

    // Provide input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    // safe cast for this small input
    defstream.avail_in = static_cast<uInt>(input.size());

    // Allocate output buffer using compressBound to be safe
    uLong bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<Bytef> compbuf(bound);
    defstream.next_out = compbuf.data();
    defstream.avail_out = static_cast<uInt>(bound);

    // Call deflate with Z_FINISH until stream ends
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // If the buffer wasn't large enough, deflate may return Z_OK; treat that as failure here
        deflateEnd(&defstream);
        return 3; // deflate didn't finish as expected
    }

    // compressed size
    uLong compressed_size = bound - defstream.avail_out;

    // Clean up deflate
    if (deflateEnd(&defstream) != Z_OK) {
        return 4; // deflateEnd failed
    }
    deflate_initialized = false;

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 5; // inflateInit failed
    }
    inflate_initialized = true;

    infstream.next_in = compbuf.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);

    // Allocate output buffer of the original size
    std::vector<Bytef> decompbuf(input.size());
    infstream.next_out = decompbuf.data();
    infstream.avail_out = static_cast<uInt>(decompbuf.size());

    // Call inflate with Z_FINISH
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Try graceful cleanup
        inflateEnd(&infstream);
        return 6; // inflate didn't finish as expected
    }

    // Clean up inflate
    if (inflateEnd(&infstream) != Z_OK) {
        return 7; // inflateEnd failed
    }
    inflate_initialized = false;

    // Verify decompressed data equals original
    if (decompbuf.size() != input.size()) {
        return 8; // size mismatch
    }
    if (std::memcmp(decompbuf.data(), input.data(), input.size()) != 0) {
        return 9; // content mismatch
    }

    // If we've reached here, everything succeeded
    return 66;
}