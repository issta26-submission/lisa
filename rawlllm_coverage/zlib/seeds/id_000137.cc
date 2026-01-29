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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress and then decompress
    const char* input = "The quick brown fox jumps over the lazy dog";
    const size_t input_len = std::strlen(input);

    // --- Compression (deflate) ---
    z_stream defst;
    std::memset(&defst, 0, sizeof(defst));
    defst.zalloc = Z_NULL;
    defst.zfree = Z_NULL;
    defst.opaque = Z_NULL;

    int ret = deflateInit(&defst, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1; // deflateInit failed
    }

    defst.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defst.avail_in = static_cast<uInt>(input_len);

    std::vector<unsigned char> compressed;
    unsigned char outbuf[128];

    // Loop until the entire input is compressed and stream is finished
    do {
        defst.next_out = outbuf;
        defst.avail_out = sizeof(outbuf);

        int flush = defst.avail_in ? Z_NO_FLUSH : Z_FINISH;
        ret = deflate(&defst, flush);
        if (ret == Z_STREAM_ERROR) { // state not clobbered, but error occurred
            deflateEnd(&defst);
            return -2;
        }

        size_t have = sizeof(outbuf) - defst.avail_out;
        compressed.insert(compressed.end(), outbuf, outbuf + have);
    } while (defst.avail_out == 0 || ret != Z_STREAM_END);

    deflateEnd(&defst);

    // --- Decompression (inflate) ---
    z_stream infst;
    std::memset(&infst, 0, sizeof(infst));
    infst.zalloc = Z_NULL;
    infst.zfree = Z_NULL;
    infst.opaque = Z_NULL;

    ret = inflateInit(&infst);
    if (ret != Z_OK) {
        return -3; // inflateInit failed
    }

    infst.next_in = compressed.empty() ? Z_NULL : compressed.data();
    infst.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decompressed;
    unsigned char outbuf2[256];

    // Loop until we reach the end of the compressed stream
    do {
        infst.next_out = outbuf2;
        infst.avail_out = sizeof(outbuf2);

        ret = inflate(&infst, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&infst);
            return -4; // inflate error
        }

        size_t have = sizeof(outbuf2) - infst.avail_out;
        decompressed.insert(decompressed.end(), outbuf2, outbuf2 + have);
    } while (ret != Z_STREAM_END);

    inflateEnd(&infst);

    // Verify result matches original input (exact bytes)
    if (decompressed.size() != input_len) {
        return -5; // size mismatch
    }
    if (std::memcmp(decompressed.data(), input, input_len) != 0) {
        return -6; // content mismatch
    }

    // Success: return 66 as requested
    return 66;
}