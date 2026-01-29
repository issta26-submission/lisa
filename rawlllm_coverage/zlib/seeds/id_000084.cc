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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input to compress/decompress
    const char* input_str = "Hello, zlib API sequence test. This is sample data to compress and decompress.";
    size_t input_len = std::strlen(input_str);

    // --- Deflate (compress) sequence ---
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    // Initialize deflate with default compression level
    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1; // deflateInit failed
    }

    // Allocate output buffer using compressBound
    uLong bound = compressBound(static_cast<uLong>(input_len));
    std::vector<unsigned char> comp(static_cast<size_t>(bound));

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input_str));
    defstream.avail_in = static_cast<uInt>(input_len);
    defstream.next_out = comp.data();
    defstream.avail_out = static_cast<uInt>(comp.size());

    // Perform compression in one call (sufficient because output buffer is large enough)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2; // compression did not finish as expected
    }

    size_t comp_size = comp.size() - defstream.avail_out;

    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return -3; // deflateEnd failed
    }

    // --- Inflate (decompress) sequence ---
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4; // inflateInit failed
    }

    // Prepare output buffer for decompressed data (we know original size)
    std::vector<unsigned char> decomp(input_len + 16); // a small slack
    infstream.next_in = comp.data();
    infstream.avail_in = static_cast<uInt>(comp_size);
    infstream.next_out = decomp.data();
    infstream.avail_out = static_cast<uInt>(decomp.size());

    // Perform decompression
    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5; // decompression did not finish correctly
    }

    size_t decomp_size = decomp.size() - infstream.avail_out;

    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return -6; // inflateEnd failed
    }

    // Verify decompressed data matches original
    if (decomp_size != input_len) {
        return -7; // size mismatch
    }
    if (std::memcmp(decomp.data(), input_str, input_len) != 0) {
        return -8; // content mismatch
    }

    // If we reach here, sequence succeeded
    return 66;
}