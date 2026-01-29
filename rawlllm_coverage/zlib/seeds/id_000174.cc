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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Example input to compress and then decompress
    const std::string input =
        "The quick brown fox jumps over the lazy dog. "
        "This is a short repeated sentence to provide some data to compress. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog.";

    // --- 1) Compress using the deflate (z_stream) API ---
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));

    // Initialize deflate with default compression level
    int ret = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1; // init failed
    }

    // Set input
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstrm.avail_in = static_cast<uInt>(input.size());

    // Allocate output buffer using compressBound to ensure it's large enough
    uLong bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<unsigned char> compbuf(bound);
    defstrm.next_out = compbuf.data();
    defstrm.avail_out = static_cast<uInt>(compbuf.size());

    // Run deflate until stream end
    ret = deflate(&defstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up before returning on error
        deflateEnd(&defstrm);
        return -2;
    }

    // Compressed size
    size_t comp_size = defstrm.total_out;

    // Clean up deflate state
    deflateEnd(&defstrm);

    // --- 2) Decompress using the inflate (z_stream) API ---
    z_stream instrm;
    std::memset(&instrm, 0, sizeof(instrm));

    ret = inflateInit(&instrm);
    if (ret != Z_OK) {
        return -3;
    }

    instrm.next_in = compbuf.data();
    instrm.avail_in = static_cast<uInt>(comp_size);

    // Allocate output buffer with the original (expected) size
    std::vector<unsigned char> decompbuf(input.size());
    instrm.next_out = decompbuf.data();
    instrm.avail_out = static_cast<uInt>(decompbuf.size());

    // Inflate in a loop until finished
    do {
        ret = inflate(&instrm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            inflateEnd(&instrm);
            return -4;
        }
        // If output buffer becomes full unexpectedly, we'd need to grow it.
        // In this test we preallocated the exact expected size so this should not be necessary.
        if (instrm.avail_out == 0) {
            inflateEnd(&instrm);
            return -5;
        }
    } while (ret == Z_OK);

    size_t decomp_size = instrm.total_out;

    // Clean up inflate state
    inflateEnd(&instrm);

    // Verify decompressed size and contents match original
    if (decomp_size != input.size()) {
        return -6;
    }
    if (std::memcmp(decompbuf.data(), input.data(), input.size()) != 0) {
        return -7;
    }

    // If we reach here, compression and decompression succeeded and verified
    return 66;
}