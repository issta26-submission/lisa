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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// A self-contained function that exercises several zlib APIs (compress2/uncompress,
// deflate/inflate with z_stream). Returns 66 on success, other negative values on error.
int test_zlib_api_sequence() {
    const char* src = "The quick brown fox jumps over the lazy dog";
    uLong src_len = (uLong)std::strlen(src) + 1; // include terminating null

    // --- 1) compress2 / uncompress (simple one-shot interfaces) ---
    uLong bound = compressBound(src_len);
    Bytef* comp1 = (Bytef*)std::malloc(bound);
    if (!comp1) return -1;

    uLongf comp1_len = (uLongf)bound;
    int ret = compress2(comp1, &comp1_len, reinterpret_cast<const Bytef*>(src), src_len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        std::free(comp1);
        return -2;
    }

    Bytef* un1 = (Bytef*)std::malloc(src_len);
    if (!un1) { std::free(comp1); return -3; }
    uLongf un1_len = (uLongf)src_len;
    ret = uncompress(un1, &un1_len, comp1, comp1_len);
    if (ret != Z_OK || un1_len != src_len || std::memcmp(un1, src, src_len) != 0) {
        std::free(comp1); std::free(un1);
        return -4;
    }
    std::free(un1);

    // --- 2) deflate (stream API) ---
    z_stream def;
    std::memset(&def, 0, sizeof(def));
    ret = deflateInit(&def, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) { std::free(comp1); return -5; }

    // Prepare input and output for deflate
    def.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(src));
    def.avail_in = static_cast<uInt>(src_len); // safe for small test data

    uLong def_bound = compressBound(src_len); // reuse compressBound for output buffer sizing
    Bytef* comp2 = (Bytef*)std::malloc(def_bound);
    if (!comp2) { deflateEnd(&def); std::free(comp1); return -6; }

    def.next_out = comp2;
    def.avail_out = static_cast<uInt>(def_bound);

    // Perform the deflate; using Z_FINISH because we provide the whole input
    ret = deflate(&def, Z_FINISH);
    if (ret != Z_STREAM_END) { deflateEnd(&def); std::free(comp1); std::free(comp2); return -7; }
    uLong comp2_len = def.total_out;
    deflateEnd(&def);

    // --- 3) inflate (stream API) ---
    z_stream inf;
    std::memset(&inf, 0, sizeof(inf));
    ret = inflateInit(&inf);
    if (ret != Z_OK) { std::free(comp1); std::free(comp2); return -8; }

    inf.next_in = comp2;
    inf.avail_in = static_cast<uInt>(comp2_len);

    Bytef* un2 = (Bytef*)std::malloc(src_len);
    if (!un2) { inflateEnd(&inf); std::free(comp1); std::free(comp2); return -9; }

    inf.next_out = un2;
    inf.avail_out = static_cast<uInt>(src_len);

    // Perform inflate; using Z_FINISH because we expect the whole stream to decompress
    ret = inflate(&inf, Z_FINISH);
    if (ret != Z_STREAM_END) { inflateEnd(&inf); std::free(comp1); std::free(comp2); std::free(un2); return -10; }
    uLong un2_len = inf.total_out;
    inflateEnd(&inf);

    if (un2_len != src_len || std::memcmp(un2, src, src_len) != 0) {
        std::free(comp1); std::free(comp2); std::free(un2);
        return -11;
    }

    // Cleanup
    std::free(comp1);
    std::free(comp2);
    std::free(un2);

    // All checks passed
    return 66;
}