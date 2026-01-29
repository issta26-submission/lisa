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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Required headers (allowed anywhere in a translation unit)
    #include <zlib.h>
    #include <vector>
    #include <cstring>

    // Example input data
    const char* src = "The quick brown fox jumps over the lazy dog. This is a test of zlib compression/decompression sequence.";
    size_t src_len = std::strlen(src);

    // Prepare compression stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstream.avail_in = static_cast<uInt>(src_len); // zlib uses uInt for avail_in

    // Allocate output buffer for compressed data (use compressBound)
    uLong bound = compressBound(static_cast<uLong>(src_len));
    std::vector<unsigned char> compBuf(bound);
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(bound);

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        deflateEnd(&defstream);
        return -1;
    }

    // Perform compression; using Z_FINISH since all input is provided
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }
    // total_out holds the size of compressed data
    size_t comp_size = defstream.total_out;
    deflateEnd(&defstream);

    // Prepare decompression stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    // Allocate output buffer for decompressed data (we expect original size)
    std::vector<unsigned char> decompBuf(src_len + 1); // +1 for potential safety
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        inflateEnd(&infstream);
        return -3;
    }

    // Perform decompression; use Z_FINISH because we expect full stream
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -4;
    }
    size_t decomp_size = infstream.total_out;
    inflateEnd(&infstream);

    // Validate decompressed size and content
    if (decomp_size != src_len) return -5;
    if (std::memcmp(decompBuf.data(), src, src_len) != 0) return -6;

    // Success: return 66 as requested
    return 66;
}