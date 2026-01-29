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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct sequence of zlib API calls:
    // - deflateInit / deflate / deflateEnd to compress data
    // - inflateInit / inflate / inflateEnd to decompress data
    // It verifies the decompressed output matches the original input and
    // returns 66 on success. On error it returns a non-66 zlib/diagnostic code.
    //
    // Note: this function uses the zlib API types and functions. Ensure you
    // include <zlib.h>, <vector>, and <cstring> in the translation unit where
    // this function is compiled.

    // Required headers (if not included elsewhere in your file, you can
    // uncomment these lines):
    // #include <zlib.h>
    // #include <vector>
    // #include <cstring>

    const char* original = "The quick brown fox jumps over the lazy dog";
    size_t original_len = std::strlen(original);

    // --- Compression (deflate) ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        // deflateInit failed
        return ret;
    }

    // Prepare input for deflate
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    defstream.avail_in = static_cast<uInt>(original_len);

    // Allocate output buffer using compressBound to be safe
    uLong bound = compressBound(static_cast<uLong>(original_len));
    std::vector<unsigned char> compBuf;
    compBuf.resize(static_cast<size_t>(bound));
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    // Run deflate until all input is consumed and stream ends
    while (true) {
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break; // finished successfully
        }
        if (ret == Z_OK) {
            // Need more output space (avail_out == 0)
            if (defstream.avail_out == 0) {
                size_t oldSize = compBuf.size();
                compBuf.resize(oldSize * 2);
                defstream.next_out = compBuf.data() + oldSize;
                // avail_out may exceed uInt, but zlib expects uInt; cap safely
                defstream.avail_out = static_cast<uInt>(compBuf.size() - oldSize);
                continue;
            } else {
                // Unexpected Z_OK with available output space: treat as error
                deflateEnd(&defstream);
                return Z_BUF_ERROR;
            }
        }
        // Other errors
        deflateEnd(&defstream);
        return ret;
    }

    // compressed size
    size_t comp_size = static_cast<size_t>(defstream.total_out);
    deflateEnd(&defstream);

    // Resize compBuf to actual compressed size
    compBuf.resize(comp_size);

    // --- Decompression (inflate) ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return ret;
    }

    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    std::vector<unsigned char> decompBuf;
    // start with a reasonable size (original length is a good starting point)
    decompBuf.resize(original_len == 0 ? 16 : original_len);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break; // finished successfully
        }
        if (ret == Z_OK) {
            // Need more output space
            if (infstream.avail_out == 0) {
                size_t oldSize = decompBuf.size();
                decompBuf.resize(oldSize * 2);
                infstream.next_out = decompBuf.data() + oldSize;
                infstream.avail_out = static_cast<uInt>(decompBuf.size() - oldSize);
                continue;
            } else {
                // Unexpected Z_OK with available output space
                inflateEnd(&infstream);
                return Z_BUF_ERROR;
            }
        }
        // Error
        inflateEnd(&infstream);
        return ret;
    }

    size_t decomp_size = static_cast<size_t>(infstream.total_out);
    inflateEnd(&infstream);

    // Resize to actual decompressed size
    decompBuf.resize(decomp_size);

    // Verify decompressed data matches the original
    if (decomp_size != original_len) {
        return -1;
    }
    if (std::memcmp(decompBuf.data(), original, original_len) != 0) {
        return -2;
    }

    // Success: return 66 as requested
    return 66;
}