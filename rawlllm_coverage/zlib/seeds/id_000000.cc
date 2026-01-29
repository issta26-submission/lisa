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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare sample input (repeated to make it compressible)
    std::string input;
    for (int i = 0; i < 64; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }

    // --- Compress using deflate ---
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return 1;
    }

    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(input.size());

    const size_t CHUNK = 16384;
    std::vector<unsigned char> compressed;
    compressed.reserve(compressBound(static_cast<uLong>(input.size())));

    unsigned char outbuf[CHUNK];
    // Feed input until consumed
    while (defstrm.avail_in > 0) {
        defstrm.next_out = outbuf;
        defstrm.avail_out = CHUNK;
        ret = deflate(&defstrm, Z_NO_FLUSH);
        if (ret != Z_OK) {
            deflateEnd(&defstrm);
            return 2;
        }
        size_t have = CHUNK - defstrm.avail_out;
        if (have > 0) {
            compressed.insert(compressed.end(), outbuf, outbuf + have);
        }
    }
    // Finish the stream
    do {
        defstrm.next_out = outbuf;
        defstrm.avail_out = CHUNK;
        ret = deflate(&defstrm, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstrm);
            return 3;
        }
        size_t have = CHUNK - defstrm.avail_out;
        if (have > 0) {
            compressed.insert(compressed.end(), outbuf, outbuf + have);
        }
    } while (ret != Z_STREAM_END);

    // Clean up deflate
    deflateEnd(&defstrm);

    // --- Decompress using inflate ---
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return 4;
    }

    infstrm.next_in = compressed.empty() ? Z_NULL : const_cast<Bytef*>(compressed.data());
    infstrm.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decompressed;
    decompressed.reserve(input.size());

    // Inflate until stream end
    int infret;
    do {
        infstrm.next_out = outbuf;
        infstrm.avail_out = CHUNK;
        infret = inflate(&infstrm, Z_NO_FLUSH);
        if (infret != Z_OK && infret != Z_STREAM_END) {
            inflateEnd(&infstrm);
            return 5;
        }
        size_t have = CHUNK - infstrm.avail_out;
        if (have > 0) {
            decompressed.insert(decompressed.end(), outbuf, outbuf + have);
        }
    } while (infret != Z_STREAM_END);

    // Clean up inflate
    inflateEnd(&infstrm);

    // Compare decompressed data with original input
    if (decompressed.size() != input.size()) {
        return 6;
    }
    if (!std::equal(decompressed.begin(), decompressed.end(),
                    reinterpret_cast<const unsigned char*>(input.data()))) {
        return 7;
    }

    // If everything succeeded, return 66 as requested
    return 66;
}