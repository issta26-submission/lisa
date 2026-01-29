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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Necessary includes for this function to be self-contained if pasted alone.
    // (Placing includes inside a function is allowed; they will be processed by the preprocessor.)
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Sample input to compress and then decompress
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a test of the zlib API sequence: deflate -> inflate.";

    // --- Compression (deflate) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return -1;
    }

    uLong sourceLen = static_cast<uLong>(input.size());
    // Allocate a buffer large enough using compressBound
    uLong bound = compressBound(sourceLen);
    std::vector<unsigned char> compBuf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(sourceLen);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    // Run deflate with Z_FINISH; expect Z_STREAM_END on completion
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }

    // Compressed size
    size_t compSize = compBuf.size() - defstream.avail_out;

    // Clean up deflate state
    deflateEnd(&defstream);

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3;
    }

    std::vector<unsigned char> decompBuf(sourceLen); // we know original size
    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compSize);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    // Run inflate with Z_FINISH; expect Z_STREAM_END on completion
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -4;
    }

    size_t decompSize = decompBuf.size() - infstream.avail_out;

    // Clean up inflate state
    inflateEnd(&infstream);

    // Verify sizes and content match the original input
    if (decompSize != input.size()) {
        return -5;
    }
    if (std::memcmp(decompBuf.data(), input.data(), input.size()) != 0) {
        return -6;
    }

    // Success: return the requested value
    return 66;
}