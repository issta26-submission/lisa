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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress/decompress
    const char input[] = "The quick brown fox jumps over the lazy dog. "
                         "This is a test of zlib deflate/inflate API sequence.";
    const size_t input_len = std::strlen(input);

    // --- 1) Compress with deflate (z_stream + deflateInit / deflate / deflateEnd) ---
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = Z_NULL;
    dstrm.zfree = Z_NULL;
    dstrm.opaque = Z_NULL;

    dstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    dstrm.avail_in = static_cast<uInt>(input_len);

    int ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // initialization failed
        return -1;
    }

    std::vector<unsigned char> compressed;
    const size_t CHUNK = 128;
    unsigned char outbuf[CHUNK];

    // Run deflate until the stream ends
    do {
        dstrm.next_out = outbuf;
        dstrm.avail_out = static_cast<uInt>(CHUNK);

        ret = deflate(&dstrm, Z_FINISH); // compress all input with Z_FINISH
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            deflateEnd(&dstrm);
            return -2;
        }

        size_t have = CHUNK - dstrm.avail_out;
        if (have > 0) {
            compressed.insert(compressed.end(), outbuf, outbuf + have);
        }
    } while (ret != Z_STREAM_END);

    // Clean up deflate state
    deflateEnd(&dstrm);

    // --- 2) Decompress with inflate (z_stream + inflateInit / inflate / inflateEnd) ---
    z_stream istrm;
    std::memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = Z_NULL;
    istrm.zfree = Z_NULL;
    istrm.opaque = Z_NULL;

    istrm.next_in = compressed.empty() ? Z_NULL : compressed.data();
    istrm.avail_in = static_cast<uInt>(compressed.size());

    ret = inflateInit(&istrm);
    if (ret != Z_OK) {
        return -3;
    }

    std::vector<unsigned char> decompressed;
    unsigned char inbuf[CHUNK];

    // Run inflate until the stream ends
    do {
        istrm.next_out = inbuf;
        istrm.avail_out = static_cast<uInt>(CHUNK);

        ret = inflate(&istrm, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&istrm);
            return -4;
        }

        size_t have = CHUNK - istrm.avail_out;
        if (have > 0) {
            decompressed.insert(decompressed.end(), inbuf, inbuf + have);
        }
    } while (ret != Z_STREAM_END);

    // Clean up inflate state
    inflateEnd(&istrm);

    // --- 3) Verify decompressed data matches original ---
    if (decompressed.size() != input_len) {
        return -5;
    }
    if (std::memcmp(decompressed.data(), input, input_len) != 0) {
        return -6;
    }

    // All zlib API usage succeeded and round-trip verified
    return 66;
}