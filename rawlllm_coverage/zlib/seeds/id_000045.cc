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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample data to compress/decompress
    const char* original = "The quick brown fox jumps over the lazy dog. "
                           "This is a sample string to exercise zlib APIs. "
                           "Repeat this sentence to make input larger. "
                           "The quick brown fox jumps over the lazy dog. "
                           "The quick brown fox jumps over the lazy dog.";
    size_t original_len = std::strlen(original);

    // 1) Use the simple convenience APIs: compress() and uncompress()
    uLongf comp_bound = compressBound(static_cast<uLong>(original_len));
    std::vector<unsigned char> comp_buf(comp_bound);
    uLongf comp_len = comp_bound;

    int ret = compress(comp_buf.data(),
                       &comp_len,
                       reinterpret_cast<const Bytef*>(original),
                       static_cast<uLong>(original_len));
    if (ret != Z_OK) return 1; // compress failed

    // Decompress with uncompress()
    std::vector<unsigned char> decomp_buf(original_len);
    uLongf decomp_len = static_cast<uLong>(original_len);
    ret = uncompress(decomp_buf.data(),
                     &decomp_len,
                     comp_buf.data(),
                     comp_len);
    if (ret != Z_OK) return 2; // uncompress failed
    if (decomp_len != original_len) return 3; // length mismatch
    if (std::memcmp(decomp_buf.data(), original, original_len) != 0) return 4; // content mismatch

    // 2) Streaming APIs: deflate (compress) with z_stream
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 5;

    // Prepare streaming buffers
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original));
    defstream.avail_in = static_cast<uInt>(original_len);

    std::vector<unsigned char> scomp_buf(compressBound(static_cast<uLong>(original_len)));
    defstream.next_out = scomp_buf.data();
    defstream.avail_out = static_cast<uInt>(scomp_buf.size());

    // Finish in one call (small input). For large input, loop until avail_in == 0 and call with Z_FINISH when done.
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&defstream); return 6; }
    size_t scomp_len = defstream.total_out;
    deflateEnd(&defstream);

    // 3) Streaming inflate (decompress) with z_stream
    z_stream instrm;
    std::memset(&instrm, 0, sizeof(instrm));
    ret = inflateInit(&instrm);
    if (ret != Z_OK) return 7;

    instrm.next_in = scomp_buf.data();
    instrm.avail_in = static_cast<uInt>(scomp_len);

    std::vector<unsigned char> sdecomp_buf(original_len);
    instrm.next_out = sdecomp_buf.data();
    instrm.avail_out = static_cast<uInt>(sdecomp_buf.size());

    ret = inflate(&instrm, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&instrm); return 8; }
    size_t sdecomp_len = instrm.total_out;
    inflateEnd(&instrm);

    if (sdecomp_len != original_len) return 9;
    if (std::memcmp(sdecomp_buf.data(), original, original_len) != 0) return 10;

    // If we reached here, all tested sequences succeeded
    return 66;
}