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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example source data (include the terminating NUL so we can verify byte-for-byte)
    const char* src = "Example data for zlib API test: compress and decompress round-trip.";
    uLong srcLen = static_cast<uLong>(std::strlen(src) + 1); // include NUL

    // 1) Initialize a deflate (compress) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1; // deflateInit failed
    }
    bool deflate_inited = true;

    // 2) Allocate output buffer for compressed data using compressBound
    uLong bound = compressBound(srcLen);
    std::vector<unsigned char> compBuf(bound);

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src));
    defstream.avail_in = srcLen;
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(bound); // uInt required for avail_out

    // 3) Perform compression in one shot (small input)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // cleanup
        deflateEnd(&defstream);
        return 2; // compression failed
    }
    uLong compSize = bound - defstream.avail_out;

    // 4) Clean up deflate
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return 3; // deflateEnd failed
    }
    deflate_inited = false;

    // 5) Initialize an inflate (decompress) stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return 4; // inflateInit failed
    }
    bool inflate_inited = true;

    // 6) Prepare output buffer for decompressed data (we know original size)
    std::vector<unsigned char> unwoundBuf(srcLen);
    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compSize);
    infstream.next_out = unwoundBuf.data();
    infstream.avail_out = static_cast<uInt>(srcLen);

    // 7) Perform inflation
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // cleanup
        inflateEnd(&infstream);
        return 5; // decompression failed
    }

    // 8) Clean up inflate
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return 6; // inflateEnd failed
    }
    inflate_inited = false;

    // 9) Verify round-trip equality (byte-for-byte)
    if (unwoundBuf.size() != static_cast<size_t>(srcLen)) {
        return 7; // unexpected size
    }
    if (std::memcmp(unwoundBuf.data(), src, srcLen) != 0) {
        return 8; // content mismatch
    }

    // All zlib operations succeeded and content matches
    return 66;
}