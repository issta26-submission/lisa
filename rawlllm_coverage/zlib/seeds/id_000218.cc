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
//<ID> 218
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a complete, correct sequence of zlib API calls:
    // - deflateInit / deflate (loop) / deflateEnd
    // - inflateInit / inflate (loop) / inflateEnd
    // It compresses a sample input, then inflates it back and verifies equality.
    //
    // Returns 66 on success, non-66 on failure.

    // Include headers locally so the function is self-contained when pasted into a file.
    // (Including headers inside a function is valid C++ though uncommon.)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring> // for std::memcmp

    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a test string for zlib compression and decompression.";
    const uLong src_len = static_cast<uLong>(input.size());

    // Determine maximum compressed size
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> comp_buf(bound);

    // Set up deflate (compress)
    z_stream defstrm{};
    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // failed to initialize deflate
        return 1;
    }

    defstrm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(src_len); // safe for small input
    defstrm.next_out = comp_buf.data();
    defstrm.avail_out = static_cast<uInt>(bound);

    // Run deflate in a loop until stream ends
    while (true) {
        ret = deflate(&defstrm, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK) {
            // need more output space (shouldn't happen here because we used compressBound),
            // but handle by enlarging the output buffer if it does.
            size_t already = defstrm.total_out;
            size_t old_size = comp_buf.size();
            size_t new_size = old_size * 2 + 1;
            comp_buf.resize(new_size);
            defstrm.next_out = comp_buf.data() + already;
            defstrm.avail_out = static_cast<uInt>(new_size - already);
            continue;
        }
        // unexpected error
        deflateEnd(&defstrm);
        return 2;
    }

    // final compressed size
    uLong comp_size = defstrm.total_out;

    // Clean up deflate stream
    if (deflateEnd(&defstrm) != Z_OK) {
        return 3;
    }

    // Prepare buffer for decompression
    std::vector<unsigned char> decomp_buf(src_len);

    // Set up inflate (decompress)
    z_stream infstrm{};
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return 4;
    }

    infstrm.next_in = comp_buf.data();
    infstrm.avail_in = static_cast<uInt>(comp_size);
    infstrm.next_out = decomp_buf.data();
    infstrm.avail_out = static_cast<uInt>(decomp_buf.size());

    // Run inflate in a loop
    while (true) {
        ret = inflate(&infstrm, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK) {
            // need more output space: enlarge the output buffer
            size_t already = infstrm.total_out;
            size_t old_size = decomp_buf.size();
            size_t new_size = old_size * 2 + 1;
            decomp_buf.resize(new_size);
            infstrm.next_out = decomp_buf.data() + already;
            infstrm.avail_out = static_cast<uInt>(new_size - already);
            continue;
        }
        // unexpected error
        inflateEnd(&infstrm);
        return 5;
    }

    // final decompressed size
    uLong decomp_size = infstrm.total_out;

    // Clean up inflate stream
    if (inflateEnd(&infstrm) != Z_OK) {
        return 6;
    }

    // Verify sizes and content
    if (decomp_size != src_len) {
        return 7;
    }
    if (std::memcmp(decomp_buf.data(), input.data(), src_len) != 0) {
        return 8;
    }

    // All good
    return 66;
}