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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input data
    const std::string original = std::string("The quick brown fox jumps over the lazy dog. ")
        + std::string("Repeat to increase size. ")
        + std::string("The quick brown fox jumps over the lazy dog. ")
        + std::string("Repeat to increase size. ");

    const size_t CHUNK = 16384;

    // --- Compression (deflate) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(original.size());

    std::vector<unsigned char> compressed;
    compressed.reserve(128);

    int ret;
    unsigned char outbuf[CHUNK];

    // Feed input and finish when all input consumed and deflate returns Z_STREAM_END
    do {
        defstream.avail_out = CHUNK;
        defstream.next_out = outbuf;
        int flush = (defstream.avail_in > 0) ? Z_NO_FLUSH : Z_FINISH;
        ret = deflate(&defstream, flush);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -2;
        }
        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);

    if (deflateEnd(&defstream) != Z_OK) {
        return -3;
    }

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.next_in = compressed.empty() ? Z_NULL : compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed.size());

    if (inflateInit(&infstream) != Z_OK) {
        return -4;
    }

    std::vector<unsigned char> decompressed;
    decompressed.reserve(original.size());

    do {
        infstream.avail_out = CHUNK;
        infstream.next_out = outbuf;
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -5;
        }
        size_t have = CHUNK - infstream.avail_out;
        decompressed.insert(decompressed.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);

    if (inflateEnd(&infstream) != Z_OK) {
        return -6;
    }

    // Verify decompressed matches original
    if (decompressed.size() != original.size()) {
        return -7;
    }
    if (std::memcmp(decompressed.data(), original.data(), original.size()) != 0) {
        return -8;
    }

    // Success: return requested value
    return 66;
}