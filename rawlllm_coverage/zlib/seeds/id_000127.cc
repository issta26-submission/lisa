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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when copied into a file.
    // (Preprocessor directives are allowed inside a function scope.)
    #include <zlib.h>
    #include <vector>
    #include <cstring>

    // Sample input to compress / decompress
    const char *input = "The quick brown fox jumps over the lazy dog";
    size_t input_len = std::strlen(input);

    // 1) Prepare compression buffer using compressBound from zlib
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> compBuf(static_cast<size_t>(bound));

    // 2) Initialize z_stream for deflate (compression)
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failure
        return -1;
    }

    // 3) Provide input and output buffers to the stream and call deflate
    defstream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    // Use Z_FINISH because we provide the whole input in one shot
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // compression didn't finish as expected
        deflateEnd(&defstream);
        return -2;
    }
    // compressed size is total buffer size minus remaining avail_out
    size_t compressed_size = compBuf.size() - defstream.avail_out;

    // 4) Clean up deflate state
    deflateEnd(&defstream);

    // 5) Prepare decompression (inflate)
    // Allocate a buffer at least as big as the original input
    std::vector<unsigned char> decompBuf(input_len + 1); // +1 for safety / possible NUL
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3;
    }

    // 6) Provide compressed data and output buffer to inflate
    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    // Call inflate in a loop until stream end
    do {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            // Error during decompression
            inflateEnd(&infstream);
            return -4;
        }
        // If output buffer fills up before finishing, that's an error for this simple test
        if (infstream.avail_out == 0 && ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -5;
        }
    } while (ret != Z_STREAM_END);

    // 7) Finalize inflate state
    size_t decompressed_size = decompBuf.size() - infstream.avail_out;
    inflateEnd(&infstream);

    // 8) Verify that decompressed data matches the original input
    if (decompressed_size != input_len) {
        return -6;
    }
    if (std::memcmp(decompBuf.data(), input, input_len) != 0) {
        return -7;
    }

    // Success: return 66 as requested
    return 66;
}