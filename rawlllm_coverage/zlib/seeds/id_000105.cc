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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare some sample data
    std::string original = "The quick brown fox jumps over the lazy dog. ";
    // make it larger so compression is meaningful
    for (int i = 0; i < 20; ++i) original += original;

    uLong src_len = static_cast<uLong>(original.size());
    if (src_len == 0) return -1;

    // 1) High-level compress/uncompress using compress2/uncompress
    uLong bound = compressBound(src_len);
    std::vector<unsigned char> comp1(bound);
    uLongf comp1_len = static_cast<uLongf>(bound);

    int rc = compress2(comp1.data(), &comp1_len,
                       reinterpret_cast<const Bytef*>(original.data()),
                       static_cast<uLong>(src_len),
                       Z_BEST_COMPRESSION);
    if (rc != Z_OK) return -2;

    std::vector<unsigned char> decomp1(src_len);
    uLongf decomp1_len = static_cast<uLongf>(src_len);
    rc = uncompress(decomp1.data(), &decomp1_len,
                    comp1.data(), static_cast<uLong>(comp1_len));
    if (rc != Z_OK) return -3;

    // Verify data round-trip
    if (decomp1_len != src_len) return -4;
    if (std::memcmp(decomp1.data(), original.data(), src_len) != 0) return -5;

    // 2) Streaming-style compression using deflateInit / deflate / deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    rc = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (rc != Z_OK) return -6;

    // Allocate output buffer using compressBound as safe upper bound
    std::vector<unsigned char> comp2(bound);
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(src_len);
    defstream.next_out = comp2.data();
    defstream.avail_out = static_cast<uInt>(comp2.size());

    rc = deflate(&defstream, Z_FINISH);
    if (rc != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -7;
    }
    uLong comp2_len = defstream.total_out;
    deflateEnd(&defstream);

    // 3) Streaming-style decompression using inflateInit / inflate / inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    rc = inflateInit(&infstream);
    if (rc != Z_OK) return -8;

    std::vector<unsigned char> decomp2(src_len);
    infstream.next_in = comp2.data();
    infstream.avail_in = static_cast<uInt>(comp2_len);
    infstream.next_out = decomp2.data();
    infstream.avail_out = static_cast<uInt>(decomp2.size());

    rc = inflate(&infstream, Z_FINISH);
    if (rc != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -9;
    }
    uLong decomp2_len = infstream.total_out;
    inflateEnd(&infstream);

    // Verify streaming round-trip
    if (decomp2_len != src_len) return -10;
    if (std::memcmp(decomp2.data(), original.data(), src_len) != 0) return -11;

    // If all checks passed, return 66 as requested
    return 66;
}