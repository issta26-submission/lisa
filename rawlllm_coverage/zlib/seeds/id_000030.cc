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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This sentence is repeated to give some data to compress. "
                              "The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog.";

    const size_t input_len = input.size();

    // --- Deflate (compress) using zlib APIs ---
    z_stream defstream{};
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return 1;
    }

    // Allocate output buffer using compressBound to guarantee enough space
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> compbuf(bound);
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = compbuf.data();
    defstream.avail_out = static_cast<uInt>(compbuf.size());

    // Perform compression with a single call since buffer is large enough
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // compression didn't finish properly
        deflateEnd(&defstream);
        return 2;
    }

    // compressed size
    size_t comp_size = compbuf.size() - defstream.avail_out;

    // Clean up deflate state
    if (deflateEnd(&defstream) != Z_OK) {
        return 3;
    }

    // --- Inflate (decompress) using zlib APIs ---
    z_stream infstream{};
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 4;
    }

    std::vector<unsigned char> decompbuf(input_len); // we know expected size
    infstream.next_in = compbuf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decompbuf.data();
    infstream.avail_out = static_cast<uInt>(decompbuf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return 5;
    }

    size_t decomp_size = decompbuf.size() - infstream.avail_out;

    if (inflateEnd(&infstream) != Z_OK) {
        return 6;
    }

    // Verify that decompressed data equals original input
    if (decomp_size != input_len) {
        return 7;
    }
    if (std::memcmp(decompbuf.data(), input.data(), input_len) != 0) {
        return 8;
    }

    // If all operations succeeded and data matches, return 66 as requested
    return 66;
}