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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input to compress and then decompress
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a test string for zlib compression/decompression.";
    // Prepare deflate (compression)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1; // deflateInit failed
    }

    // Set input for compression
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    std::vector<unsigned char> comp_buffer(128);
    std::vector<unsigned char> compressed; // will hold compressed data

    // Compress until deflate returns Z_STREAM_END
    do {
        defstream.next_out = comp_buffer.data();
        defstream.avail_out = static_cast<uInt>(comp_buffer.size());

        ret = deflate(&defstream, Z_FINISH); // finish in one go (loop handles repeated calls)
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return 2; // deflate error
        }

        std::size_t have = comp_buffer.size() - defstream.avail_out;
        compressed.insert(compressed.end(), comp_buffer.data(), comp_buffer.data() + have);
    } while (ret != Z_STREAM_END);

    // Clean up compressor
    if (deflateEnd(&defstream) != Z_OK) {
        return 3; // deflateEnd failed
    }

    // Prepare inflate (decompression)
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 4; // inflateInit failed
    }

    // Set input for decompression
    infstream.next_in = compressed.empty() ? Z_NULL : reinterpret_cast<Bytef*>(compressed.data());
    infstream.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decomp_buffer(128);
    std::vector<unsigned char> decompressed;

    // Decompress until inflate returns Z_STREAM_END
    do {
        infstream.next_out = decomp_buffer.data();
        infstream.avail_out = static_cast<uInt>(decomp_buffer.size());

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return 5; // inflate error
        }

        std::size_t have = decomp_buffer.size() - infstream.avail_out;
        decompressed.insert(decompressed.end(), decomp_buffer.data(), decomp_buffer.data() + have);
    } while (ret != Z_STREAM_END);

    // Clean up inflater
    if (inflateEnd(&infstream) != Z_OK) {
        return 6; // inflateEnd failed
    }

    // Verify decompressed data matches original
    if (decompressed.size() != input.size()) {
        return 7; // size mismatch
    }
    if (std::memcmp(decompressed.data(), input.data(), input.size()) != 0) {
        return 8; // content mismatch
    }

    // Success
    return 66;
}