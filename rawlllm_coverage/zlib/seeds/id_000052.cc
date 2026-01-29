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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input data to compress and then decompress
    const char input[] = "The quick brown fox jumps over the lazy dog. zlib test sequence.";
    uLong input_len = static_cast<uLong>(std::strlen(input));

    // Prepare compression stream
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));

    // Allocate output buffer based on compressBound
    uLong bound = compressBound(input_len);
    std::vector<Bytef> comp_buf(bound);

    // Initialize deflate (compression)
    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1;
    }

    // Set input and output for compression
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    defstrm.avail_in = static_cast<uInt>(input_len);
    defstrm.next_out = comp_buf.data();
    defstrm.avail_out = static_cast<uInt>(comp_buf.size());

    // Perform compression with Z_FINISH to finish in one shot (buffer is large enough)
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up and return error
        deflateEnd(&defstrm);
        return 2;
    }

    // Get compressed size and clean up deflate
    uLong comp_size = defstrm.total_out;
    deflateEnd(&defstrm);

    // Prepare decompression stream
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));

    // Allocate output buffer for decompressed data (should be at least input_len)
    std::vector<Bytef> decomp_buf(input_len);

    // Initialize inflate (decompression)
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return 3;
    }

    // Set input (compressed data) and output (decompressed buffer)
    infstrm.next_in = comp_buf.data();
    infstrm.avail_in = static_cast<uInt>(comp_size);
    infstrm.next_out = decomp_buf.data();
    infstrm.avail_out = static_cast<uInt>(decomp_buf.size());

    // Perform inflation
    ret = inflate(&infstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstrm);
        return 4;
    }

    // Get decompressed size and clean up inflate
    uLong decomp_size = infstrm.total_out;
    inflateEnd(&infstrm);

    // Verify decompressed size and content matches original
    if (decomp_size != input_len) {
        return 5;
    }
    if (std::memcmp(decomp_buf.data(), input, input_len) != 0) {
        return 6;
    }

    // All zlib operations succeeded and data round-tripped correctly
    return 66;
}