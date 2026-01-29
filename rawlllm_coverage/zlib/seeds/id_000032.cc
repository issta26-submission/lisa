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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress / decompress
    const std::string input = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string used to validate zlib compression and decompression. "
        "Repeating the sentence to make data more compressible. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. ";

    // --- DEFLATE (compress) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));

    // Initialize for compression at default compression level
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    // Allocate output buffer using compressBound to be safe
    uLong bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<unsigned char> compressed;
    compressed.resize(bound);

    defstream.next_out = compressed.data();
    defstream.avail_out = static_cast<uInt>(compressed.size());

    int def_ret = deflate(&defstream, Z_FINISH);
    if (def_ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -1;
    }

    // Actual compressed size:
    size_t compressed_size = compressed.size() - defstream.avail_out;

    // Clean up deflate state
    deflateEnd(&defstream);

    // --- INFLATE (decompress) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));

    if (inflateInit(&infstream) != Z_OK) {
        return -1;
    }

    infstream.next_in = compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);

    // Allocate output buffer at least as big as original input
    std::vector<unsigned char> decompressed;
    decompressed.resize(input.size());

    infstream.next_out = decompressed.data();
    infstream.avail_out = static_cast<uInt>(decompressed.size());

    int inf_ret = inflate(&infstream, Z_FINISH);
    if (inf_ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -1;
    }

    size_t decompressed_size = decompressed.size() - infstream.avail_out;

    // Clean up inflate state
    inflateEnd(&infstream);

    // Verify decompressed output matches the original input
    if (decompressed_size != input.size()) {
        return -1;
    }
    if (std::memcmp(decompressed.data(), input.data(), input.size()) != 0) {
        return -1;
    }

    // If everything succeeded, return 66 as requested
    return 66;
}