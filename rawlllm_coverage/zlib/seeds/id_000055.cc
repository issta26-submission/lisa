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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // include headers inside function to keep the function self-contained
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    const std::string input =
        "The quick brown fox jumps over the lazy dog. "
        "This is a short test string to validate zlib deflate/inflate sequence.";

    const int CHUNK = 16384;
    int ret;

    // --- Compress using deflate ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 1;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    std::vector<unsigned char> compressed;
    unsigned char outbuf[CHUNK];

    // Feed input
    while (defstream.avail_in > 0) {
        defstream.next_out = outbuf;
        defstream.avail_out = CHUNK;
        ret = deflate(&defstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) { deflateEnd(&defstream); return 2; }
        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), outbuf, outbuf + have);
    }

    // Finish compression
    for (;;) {
        defstream.next_out = outbuf;
        defstream.avail_out = CHUNK;
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_ERROR) { deflateEnd(&defstream); return 3; }
        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), outbuf, outbuf + have);
        if (ret == Z_STREAM_END) break;
        // otherwise ret should be Z_OK and loop continues
    }

    deflateEnd(&defstream);

    // --- Decompress using inflate ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return 4;

    infstream.next_in = compressed.empty() ? nullptr : reinterpret_cast<Bytef*>(compressed.data());
    infstream.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decompressed;
    unsigned char inbuf[CHUNK];

    for (;;) {
        infstream.next_out = inbuf;
        infstream.avail_out = CHUNK;
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&infstream);
            return 5;
        }
        size_t have = CHUNK - infstream.avail_out;
        decompressed.insert(decompressed.end(), inbuf, inbuf + have);
        if (ret == Z_STREAM_END) break;
        // If ret == Z_OK, loop continues until stream end
    }

    inflateEnd(&infstream);

    // Verify decompressed equals original
    std::string output(reinterpret_cast<char*>(decompressed.data()), decompressed.size());
    if (output != input) return 6;

    // Success as requested
    return 66;
}