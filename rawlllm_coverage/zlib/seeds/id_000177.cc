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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct usage sequence of several zlib APIs:
    // - compressBound to estimate max compressed size
    // - compress2 to compress data
    // - uncompress to decompress data
    // It verifies the decompressed output equals the original input and returns 66 on success.
    //
    // Note: This function assumes the zlib header (<zlib.h>) and the standard C functions
    // (strlen, strcmp, malloc, free) are available in the compilation unit.

    const char *original = "The quick brown fox jumps over the lazy dog. "
                           "This sentence is repeated to provide some data for compression. "
                           "The quick brown fox jumps over the lazy dog. "
                           "The quick brown fox jumps over the lazy dog.";

    // Get source length
    uLong src_len = (uLong)strlen(original);

    // Get an upper bound on the compressed size
    uLong comp_bound = compressBound(src_len);

    // Allocate buffer for compressed data
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    if (comp_buf == NULL) {
        return -1; // allocation failure
    }

    // Perform compression using best compression level
    uLong comp_len = comp_bound;
    int c_res = compress2(comp_buf, &comp_len, (const Bytef *)original, src_len, Z_BEST_COMPRESSION);
    if (c_res != Z_OK) {
        free(comp_buf);
        return c_res; // propagate zlib error
    }

    // Allocate buffer for decompressed data (we know original size)
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 1)); // +1 for terminating NUL
    if (decomp_buf == NULL) {
        free(comp_buf);
        return -2; // allocation failure
    }

    // Perform decompression
    uLong decomp_len = src_len;
    int u_res = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    if (u_res != Z_OK) {
        free(comp_buf);
        free(decomp_buf);
        return u_res; // propagate zlib error
    }

    // Null-terminate and verify the decompressed data matches the original
    decomp_buf[decomp_len] = 0;
    if (decomp_len != src_len || strcmp((const char *)decomp_buf, original) != 0) {
        free(comp_buf);
        free(decomp_buf);
        return -3; // verification failure
    }

    // Clean up
    free(comp_buf);
    free(decomp_buf);

    // Success as requested
    return 66;
}