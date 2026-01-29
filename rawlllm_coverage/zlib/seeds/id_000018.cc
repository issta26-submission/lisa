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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example source data to compress/decompress
    const char* src = "The quick brown fox jumps over the lazy dog";
    uLong src_len = static_cast<uLong>(std::strlen(src));

    // --- Compression (deflate) ---
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // Start with compressBound to get a safe initial output buffer size
    std::vector<unsigned char> compbuf(static_cast<size_t>(compressBound(src_len)));
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstrm.avail_in = src_len;
    defstrm.next_out = compbuf.data();
    defstrm.avail_out = static_cast<uInt>(compbuf.size());

    // Loop until deflate signals Z_STREAM_END
    for (;;) {
        ret = deflate(&defstrm, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret != Z_OK && ret != Z_BUF_ERROR) {
            deflateEnd(&defstrm);
            return -1;
        }
        // Need more output space: grow buffer
        size_t old_size = compbuf.size();
        size_t new_size = (old_size == 0) ? 256 : old_size * 2;
        compbuf.resize(new_size);
        defstrm.next_out = compbuf.data() + defstrm.total_out;
        defstrm.avail_out = static_cast<uInt>(new_size - defstrm.total_out);
    }

    uLong comp_len = defstrm.total_out;
    deflateEnd(&defstrm);

    // --- Decompression (inflate) ---
    z_stream instrm;
    std::memset(&instrm, 0, sizeof(instrm));
    ret = inflateInit(&instrm);
    if (ret != Z_OK) {
        return -1;
    }

    // Allocate an output buffer (we expect the original size)
    std::vector<unsigned char> outbuf(static_cast<size_t>(src_len));
    instrm.next_in = compbuf.data();
    instrm.avail_in = static_cast<uInt>(comp_len);
    instrm.next_out = outbuf.data();
    instrm.avail_out = static_cast<uInt>(outbuf.size());

    for (;;) {
        ret = inflate(&instrm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret != Z_OK && ret != Z_BUF_ERROR) {
            inflateEnd(&instrm);
            return -1;
        }
        // Need more output space (rare here, but handle it)
        size_t old_size = outbuf.size();
        size_t new_size = (old_size == 0) ? 256 : old_size * 2;
        outbuf.resize(new_size);
        instrm.next_out = outbuf.data() + instrm.total_out;
        instrm.avail_out = static_cast<uInt>(new_size - instrm.total_out);
    }

    uLong out_len = instrm.total_out;
    inflateEnd(&instrm);

    // Verify decompressed data matches original
    if (out_len != src_len) {
        return -1;
    }
    if (std::memcmp(outbuf.data(), src, src_len) != 0) {
        return -1;
    }

    // If everything succeeded, return the required value
    return 66;
}