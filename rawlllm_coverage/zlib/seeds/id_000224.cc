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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// Compress a small buffer with the zlib deflate stream API and then
// decompress it with the inflate stream API. Return 66 on success.
int test_zlib_api_sequence() {
    const char* orig = "The quick brown fox jumps over the lazy dog";
    size_t orig_len = std::strlen(orig);

    // --- Deflate (compress) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree  = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return ret; // initialization failure
    }

    // Provide the entire input at once
    defstream.next_in  = reinterpret_cast<Bytef*>(const_cast<char*>(orig));
    defstream.avail_in = static_cast<uInt>(orig_len);

    const size_t CHUNK = 16384;
    std::vector<unsigned char> compressed;

    // Run deflate() with Z_FINISH until the stream ends
    do {
        unsigned char outbuf[CHUNK];
        defstream.next_out  = outbuf;
        defstream.avail_out = CHUNK;

        ret = deflate(&defstream, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return ret; // compression error
        }

        size_t have = CHUNK - defstream.avail_out;
        compressed.insert(compressed.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);

    deflateEnd(&defstream);

    // --- Inflate (decompress) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree  = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return ret; // initialization failure
    }

    infstream.next_in  = compressed.empty() ? Z_NULL : compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decompressed;

    // Run inflate() until stream ends
    do {
        unsigned char outbuf[CHUNK];
        infstream.next_out  = outbuf;
        infstream.avail_out = CHUNK;

        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return ret; // decompression error
        }

        size_t have = CHUNK - infstream.avail_out;
        decompressed.insert(decompressed.end(), outbuf, outbuf + have);
    } while (ret != Z_STREAM_END);

    inflateEnd(&infstream);

    // --- Verify that decompressed data matches original ---
    if (decompressed.size() != orig_len) {
        return 1; // size mismatch
    }
    if (std::memcmp(decompressed.data(), orig, orig_len) != 0) {
        return 2; // content mismatch
    }

    // Success: return 66 as requested
    return 66;
}