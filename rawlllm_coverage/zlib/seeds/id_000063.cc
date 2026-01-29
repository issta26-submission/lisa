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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress
    const std::string original = "The quick brown fox jumps over the lazy dog. "
                                 "This is a sample string to demonstrate zlib "
                                 "compression and decompression using the deflate/"
                                 "inflate stream APIs and the convenience APIs.";

    // -------------------------
    // 1) Stream-based deflate
    // -------------------------
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    defstream.zalloc = Z_NULL;
    defstream.zfree  = Z_NULL;
    defstream.opaque = Z_NULL;

    int ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        (void)deflateEnd(&defstream);
        return -1;
    }

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(original.size());

    // allocate output using compressBound
    uLong bound = compressBound(static_cast<uLong>(original.size()));
    std::vector<unsigned char> comp_buf(bound);
    defstream.next_out = comp_buf.data();
    defstream.avail_out = static_cast<uInt>(comp_buf.size());

    // finish compression
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }

    std::size_t comp_size = comp_buf.size() - defstream.avail_out;
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) {
        return -3;
    }

    // -------------------------
    // 2) Stream-based inflate
    // -------------------------
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    infstream.zalloc = Z_NULL;
    infstream.zfree  = Z_NULL;
    infstream.opaque = Z_NULL;

    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        return -4;
    }

    infstream.next_in = comp_buf.data();
    infstream.avail_in = static_cast<uInt>(comp_size);

    std::vector<unsigned char> decomp_buf(original.size());
    infstream.next_out = decomp_buf.data();
    infstream.avail_out = static_cast<uInt>(decomp_buf.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5;
    }

    std::size_t decomp_size = decomp_buf.size() - infstream.avail_out;
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) {
        return -6;
    }

    // Verify round-trip equality for stream API
    if (decomp_size != original.size()) return -7;
    if (std::memcmp(decomp_buf.data(), original.data(), original.size()) != 0) return -8;

    // -------------------------
    // 3) Convenience APIs: compress2 / uncompress
    // -------------------------
    uLong comp2_bound = compressBound(static_cast<uLong>(original.size()));
    std::vector<unsigned char> comp2_buf(comp2_bound);
    uLong comp2_size = comp2_bound;

    // compress2 lets us set a level
    ret = compress2(comp2_buf.data(), &comp2_size,
                    reinterpret_cast<const Bytef*>(original.data()),
                    static_cast<uLong>(original.size()),
                    Z_BEST_SPEED);
    if (ret != Z_OK) {
        return -9;
    }

    // prepare buffer for uncompress
    std::vector<unsigned char> uncmp_buf(original.size());
    uLong uncmp_size = static_cast<uLong>(uncmp_buf.size());

    ret = uncompress(uncmp_buf.data(), &uncmp_size,
                     comp2_buf.data(), comp2_size);
    if (ret != Z_OK) {
        return -10;
    }

    if (uncmp_size != original.size()) return -11;
    if (std::memcmp(uncmp_buf.data(), original.data(), original.size()) != 0) return -12;

    // If we reach here, both stream and convenience APIs worked correctly.
    return 66;
}