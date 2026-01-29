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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input to compress and decompress
    const char input[] = "The quick brown fox jumps over the lazy dog. zlib round-trip test.";
    uLong input_len = static_cast<uLong>(std::strlen(input));

    // Prepare deflate (compression) stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // Allocate output buffer large enough using compressBound
    uLong bound = compressBound(input_len);
    Bytef* comp_buf = static_cast<Bytef*>(std::malloc(bound));
    if (!comp_buf) {
        deflateEnd(&defstream);
        return -2;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = comp_buf;
    defstream.avail_out = static_cast<uInt>(bound);

    // Perform compression (single call is sufficient because buffer >= compressBound)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up
        deflateEnd(&defstream);
        std::free(comp_buf);
        return -3;
    }

    uLong comp_size = defstream.total_out;
    deflateEnd(&defstream);

    // Prepare inflate (decompression) stream
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        std::free(comp_buf);
        return -4;
    }

    // Allocate buffer for decompressed data (we know original length)
    Bytef* decomp_buf = static_cast<Bytef*>(std::malloc(input_len));
    if (!decomp_buf) {
        inflateEnd(&infstream);
        std::free(comp_buf);
        return -5;
    }

    infstream.next_in = comp_buf;
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decomp_buf;
    infstream.avail_out = static_cast<uInt>(input_len);

    // Perform decompression
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        std::free(comp_buf);
        std::free(decomp_buf);
        return -6;
    }

    uLong decomp_size = infstream.total_out;
    inflateEnd(&infstream);

    // Verify round-trip equality
    bool equal = (decomp_size == input_len) && (std::memcmp(decomp_buf, input, input_len) == 0);

    // Clean up buffers
    std::free(comp_buf);
    std::free(decomp_buf);

    if (!equal) {
        return -7;
    }

    // Success: return 66 as requested
    return 66;
}