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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a typical zlib API sequence:
    //  - deflateInit / deflate (looping if necessary) / deflateEnd
    //  - inflateInit / inflate (looping if necessary) / inflateEnd
    // It compresses an in-memory buffer and then decompresses it,
    // verifies the round-trip equality, and returns 66 on success.
    #
    // Note: this implementation uses only standard zlib APIs and
    // allocates extra output space as needed.

    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    const std::string original = "The quick brown fox jumps over the lazy dog. "
                                 "This is a test string for zlib compression/decompression.";

    // --- Compress with deflate ---
    z_stream deflate_stream;
    std::memset(&deflate_stream, 0, sizeof(deflate_stream));

    int ret = deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // initialization failed
        return -1;
    }

    deflate_stream.next_in = (Bytef*)original.data();
    deflate_stream.avail_in = static_cast<uInt>(original.size());

    // Start with compressBound to ensure we have enough space (but still handle expansion)
    uLong bound = compressBound(static_cast<uLong>(original.size()));
    std::vector<unsigned char> comp;
    comp.resize(bound);

    deflate_stream.next_out = comp.data();
    deflate_stream.avail_out = static_cast<uInt>(comp.size());

    // Run deflate in a loop to handle unexpected needs for more output space
    while (true) {
        ret = deflate(&deflate_stream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            // finished compression
            break;
        } else if (ret == Z_OK) {
            // need more output space: expand buffer
            size_t used = deflate_stream.total_out;
            size_t add = comp.size(); // double current capacity
            comp.resize(comp.size() + add);
            deflate_stream.next_out = comp.data() + used;
            // avail_out must fit into uInt
            deflate_stream.avail_out = static_cast<uInt>(comp.size() - used);
            continue;
        } else {
            // error
            deflateEnd(&deflate_stream);
            return -2;
        }
    }

    size_t compressed_size = deflate_stream.total_out;
    deflateEnd(&deflate_stream);

    // shrink compressed buffer to actual size
    comp.resize(compressed_size);

    // --- Decompress with inflate ---
    z_stream inflate_stream;
    std::memset(&inflate_stream, 0, sizeof(inflate_stream));

    ret = inflateInit(&inflate_stream);
    if (ret != Z_OK) {
        return -3;
    }

    inflate_stream.next_in = comp.data();
    inflate_stream.avail_in = static_cast<uInt>(comp.size());

    std::vector<unsigned char> decomp;
    decomp.resize(original.size()); // start with the original size

    inflate_stream.next_out = decomp.data();
    inflate_stream.avail_out = static_cast<uInt>(decomp.size());

    while (true) {
        ret = inflate(&inflate_stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            // finished decompression
            break;
        } else if (ret == Z_OK) {
            // need more output space
            size_t used = inflate_stream.total_out;
            size_t add = decomp.size() ? decomp.size() : 256;
            decomp.resize(decomp.size() + add);
            inflate_stream.next_out = decomp.data() + used;
            inflate_stream.avail_out = static_cast<uInt>(decomp.size() - used);
            continue;
        } else {
            // error
            inflateEnd(&inflate_stream);
            return -4;
        }
    }

    size_t decompressed_size = inflate_stream.total_out;
    inflateEnd(&inflate_stream);

    // shrink to actual size
    decomp.resize(decompressed_size);

    // Verify decompressed data matches original
    if (decompressed_size != original.size()) return -5;
    if (std::memcmp(decomp.data(), original.data(), original.size()) != 0) return -6;

    // Success: return 66 as requested
    return 66;
}