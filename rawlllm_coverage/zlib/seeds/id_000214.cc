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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input data to compress and then decompress
    const std::string original = 
        "The quick brown fox jumps over the lazy dog. "
        "This is a test string for zlib compression/decompression. "
        "Repeat to enlarge the buffer: "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // --- Compression using deflate ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return 1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(original.size());

    const size_t CHUNK = 16384;
    std::vector<unsigned char> outchunk(CHUNK);
    std::vector<unsigned char> compressed;

    // We provide all input, then finish
    do {
        defstream.next_out = outchunk.data();
        defstream.avail_out = static_cast<uInt>(outchunk.size());

        // Use Z_FINISH to indicate we provided all input
        ret = deflate(&defstream, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return 2;
        }

        size_t have = outchunk.size() - defstream.avail_out;
        compressed.insert(compressed.end(), outchunk.begin(), outchunk.begin() + have);
    } while (ret != Z_STREAM_END);

    // Clean up deflate state
    deflateEnd(&defstream);

    // --- Decompression using inflate ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 3;
    }

    infstream.next_in = compressed.empty() ? nullptr : reinterpret_cast<Bytef*>(compressed.data());
    infstream.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> inchunk(CHUNK);
    std::vector<unsigned char> decompressed;

    do {
        infstream.next_out = inchunk.data();
        infstream.avail_out = static_cast<uInt>(inchunk.size());

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return 4;
        }

        size_t have = inchunk.size() - infstream.avail_out;
        decompressed.insert(decompressed.end(), inchunk.begin(), inchunk.begin() + have);
    } while (ret != Z_STREAM_END);

    // Clean up inflate state
    inflateEnd(&infstream);

    // Convert decompressed bytes back to std::string and compare
    std::string recovered(decompressed.begin(), decompressed.end());
    if (recovered != original) {
        // Data mismatch
        return 5;
    }

    // Success: return 66 as requested
    return 66;
}