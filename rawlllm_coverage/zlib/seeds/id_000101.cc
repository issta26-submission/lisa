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
//<ID> 101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input data to compress and then decompress
    const char* input = "The quick brown fox jumps over the lazy dog";
    size_t input_len = std::strlen(input);

    // --- 1) Compress with deflate ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return ret;
    }

    // Allocate output buffer large enough for compressed data
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> compBuf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    // Finish the compression in one call (input provided completely)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return ret;
    }
    size_t comp_size = compBuf.size() - defstream.avail_out;

    // Clean up deflate state
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return ret;
    }

    // --- 2) Decompress with inflate ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return ret;
    }

    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    // Output buffer sized to original input length + 1 for safety (not strictly needed here)
    std::vector<unsigned char> decompBuf(input_len + 1);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    // Inflate entire stream
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return ret;
    }
    size_t decomp_size = decompBuf.size() - infstream.avail_out;

    // Clean up inflate state
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return ret;
    }

    // Verify decompressed data matches original
    if (decomp_size != input_len) {
        return -1;
    }
    if (std::memcmp(decompBuf.data(), input, input_len) != 0) {
        return -2;
    }

    // Success: return 66 as requested
    return 66;
}