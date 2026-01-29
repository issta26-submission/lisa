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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input data (repeated to make it compressible)
    std::string input;
    for (int i = 0; i < 16; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    size_t input_size = input.size();

    // Prepare z_stream for deflation (compression)
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1; // init failed
    }

    // Allocate output buffer large enough for compressed data
    uLong bound = compressBound((uLong)input_size);
    std::vector<unsigned char> compbuf(bound);

    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(input_size);
    defstrm.next_out = compbuf.data();
    defstrm.avail_out = static_cast<uInt>(compbuf.size());

    // Perform compression using streaming API
    while (true) {
        ret = deflate(&defstrm, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break;
        } else if (ret != Z_OK) {
            deflateEnd(&defstrm);
            return 2; // compression error
        }
        // If Z_OK returned, but not finished, we'd normally provide more output buffer.
        // However, using compressBound sized buffer should normally avoid that.
    }

    // Calculate compressed size
    size_t comp_size = compbuf.size() - defstrm.avail_out;

    // Clean up deflate state
    if (deflateEnd(&defstrm) != Z_OK) {
        return 3;
    }

    // Prepare z_stream for inflation (decompression)
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    ret = inflateInit(&infstrm);
    if (ret != Z_OK) {
        return 4;
    }

    std::vector<unsigned char> decompbuf(input_size + 1); // +1 for safety / null-termination if needed
    infstrm.next_in = compbuf.data();
    infstrm.avail_in = static_cast<uInt>(comp_size);
    infstrm.next_out = decompbuf.data();
    infstrm.avail_out = static_cast<uInt>(decompbuf.size());

    // Perform decompression using streaming API
    while (true) {
        ret = inflate(&infstrm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break;
        } else if (ret != Z_OK) {
            inflateEnd(&infstrm);
            return 5; // decompression error
        }
        // If output buffer were too small, we'd expand it and continue.
        // Our buffer is sized to original input so this should not be necessary.
    }

    // Calculate decompressed size
    size_t decomp_size = decompbuf.size() - infstrm.avail_out;

    // Clean up inflate state
    if (inflateEnd(&infstrm) != Z_OK) {
        return 6;
    }

    // Verify decompressed data matches original
    if (decomp_size != input_size) {
        return 7; // size mismatch
    }
    if (std::memcmp(decompbuf.data(), input.data(), input_size) != 0) {
        return 8; // content mismatch
    }

    // All zlib API usage succeeded and data round-tripped correctly
    return 66;
}