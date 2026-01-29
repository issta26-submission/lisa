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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a full compress/decompress sequence using zlib's
    // deflate/inflate stream APIs. It returns 66 on success (as requested).
    //
    // Note: For compilation this function requires the zlib header and the C++
    // standard headers below. If you place this function into a file, ensure:
    // #include <zlib.h>
    // #include <vector>
    // #include <string>
    // #include <cstring>
    // are present at top-level of that file.
    //
    // The #includes are not repeated here to keep the function body focused.

    // Prepare test data
    const std::string original =
        "The quick brown fox jumps over the lazy dog. "
        "This is a test of zlib deflate/inflate stream APIs. "
        "Repeat to ensure compression has some effect. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // Convert to bytes
    const unsigned char* input_data = reinterpret_cast<const unsigned char*>(original.data());
    const size_t input_size = original.size();

    // Allocate a buffer for compressed data using compressBound to be safe.
    // compressBound takes uLong (unsigned long) and returns uLong; cast accordingly.
    uLong bound = compressBound(static_cast<uLong>(input_size));
    std::vector<unsigned char> compressed(static_cast<size_t>(bound));

    int zret = Z_OK;
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));

    // Initialize deflate (compression) stream
    defstream.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(input_data));
    defstream.avail_in = static_cast<uInt>(input_size); // safe for small inputs
    defstream.next_out = reinterpret_cast<Bytef*>(compressed.data());
    defstream.avail_out = static_cast<uInt>(compressed.size());

    zret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (zret != Z_OK) {
        // failed to initialize
        return -1;
    }

    // Perform compression (one-shot)
    zret = deflate(&defstream, Z_FINISH);
    if (zret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }

    // Get compressed size
    uLong compressed_size = defstream.total_out;

    // Clean up deflate state
    deflateEnd(&defstream);

    // Now decompress using inflate stream API
    std::vector<unsigned char> decompressed(input_size); // should match original size
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));

    infstream.next_in = reinterpret_cast<Bytef*>(compressed.data());
    infstream.avail_in = static_cast<uInt>(compressed_size);
    infstream.next_out = reinterpret_cast<Bytef*>(decompressed.data());
    infstream.avail_out = static_cast<uInt>(decompressed.size());

    zret = inflateInit(&infstream);
    if (zret != Z_OK) {
        return -3;
    }

    // Perform decompression (one-shot)
    zret = inflate(&infstream, Z_FINISH);
    if (zret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -4;
    }

    uLong decompressed_size = infstream.total_out;
    inflateEnd(&infstream);

    // Validate sizes and contents
    if (decompressed_size != input_size) {
        return -5;
    }

    if (std::memcmp(decompressed.data(), input_data, input_size) != 0) {
        return -6;
    }

    // If we reached here, compression and decompression succeeded and data matches
    return 66;
}