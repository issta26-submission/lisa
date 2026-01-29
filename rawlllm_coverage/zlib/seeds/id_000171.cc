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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data (repeated to get decent compression)
    const std::string input =
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. ";

    // --- Compression (deflate) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree  = Z_NULL;
    defstream.opaque = Z_NULL;

    // Initialize deflate
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1; // deflateInit failed
    }

    std::vector<unsigned char> compressed;
    const size_t CHUNK = 16384;
    unsigned char outbuf[CHUNK];

    // Provide the entire input as input stream
    defstream.next_in  = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    // Deflate until stream ends
    do {
        defstream.next_out  = outbuf;
        defstream.avail_out = static_cast<uInt>(CHUNK);

        ret = deflate(&defstream, Z_FINISH); // we provided all input in one go
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&defstream);
            return 2; // deflate failed
        }

        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);

    // Clean up deflate
    deflateEnd(&defstream);

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree  = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 3; // inflateInit failed
    }

    std::vector<unsigned char> decompressed;
    unsigned char inbuf[CHUNK];

    // Set compressed data as input
    if (!compressed.empty()) {
        infstream.next_in  = compressed.data();
        infstream.avail_in = static_cast<uInt>(compressed.size());
    } else {
        infstream.next_in  = Z_NULL;
        infstream.avail_in = 0;
    }

    // Inflate until stream ends
    do {
        infstream.next_out  = inbuf;
        infstream.avail_out = static_cast<uInt>(CHUNK);

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR || ret == Z_STREAM_ERROR) {
            inflateEnd(&infstream);
            return 4; // inflate failed
        }

        size_t have = CHUNK - infstream.avail_out;
        decompressed.insert(decompressed.end(), inbuf, inbuf + have);
    } while (ret != Z_STREAM_END);

    // Clean up inflate
    inflateEnd(&infstream);

    // Verify decompressed data equals original input
    std::string output(decompressed.begin(), decompressed.end());
    if (output != input) {
        return 5; // round-trip mismatch
    }

    // Success: return 66 as requested
    return 66;
}