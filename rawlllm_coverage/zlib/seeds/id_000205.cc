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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress and then decompress
    const std::string input = "The quick brown fox jumps over the lazy dog. "
                              "This is a longer test input to ensure compression "
                              "and decompression paths are exercised properly.";

    // --- DEFLATE (compress) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // Initialize deflate with default parameters (using best compression level)
    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // Provide input
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());

    // Prepare an output buffer using compressBound as a starting point
    uLong bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<unsigned char> compBuf(bound);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    // Run deflate until stream end
    while (true) {
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break; // finished successfully
        }
        if (ret != Z_OK) {
            deflateEnd(&defstream);
            return -2;
        }
        // Output buffer was too small; expand and continue
        size_t oldSize = compBuf.size();
        compBuf.resize(oldSize * 2);
        defstream.next_out = compBuf.data() + defstream.total_out;
        defstream.avail_out = static_cast<uInt>(compBuf.size() - defstream.total_out);
    }

    size_t compressedSize = static_cast<size_t>(defstream.total_out);
    deflateEnd(&defstream);

    // --- INFLATE (decompress) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compressedSize);

    // Start with an output buffer sized to the original input
    std::vector<unsigned char> decompBuf(input.size());
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -3;
    }

    // Inflate until stream end
    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break; // decompression finished
        }
        if (ret == Z_OK) {
            // need more output space?
            if (infstream.avail_out == 0) {
                size_t oldSize = decompBuf.size();
                decompBuf.resize(oldSize * 2);
                infstream.next_out = decompBuf.data() + infstream.total_out;
                infstream.avail_out = static_cast<uInt>(decompBuf.size() - infstream.total_out);
                continue;
            } else {
                // still OK but no progress? continue loop
                continue;
            }
        }
        if (ret == Z_BUF_ERROR) {
            // likely need more output space
            size_t oldSize = decompBuf.size();
            decompBuf.resize(oldSize * 2);
            infstream.next_out = decompBuf.data() + infstream.total_out;
            infstream.avail_out = static_cast<uInt>(decompBuf.size() - infstream.total_out);
            continue;
        }
        // any other error
        inflateEnd(&infstream);
        return -4;
    }

    size_t decompressedSize = static_cast<size_t>(infstream.total_out);
    inflateEnd(&infstream);

    // Verify decompressed content matches original
    if (decompressedSize != input.size()) {
        return -5;
    }
    if (std::memcmp(decompBuf.data(), input.data(), input.size()) != 0) {
        return -6;
    }

    // If we reach here, compression+decompression succeeded
    return 66;
}