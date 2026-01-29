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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream src;
    z_stream dst;
    z_stream inf;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    memset(&inf, 0, sizeof(inf));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'Z', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)(bound * 2));
    memset(comp, 0, (size_t)(bound * 2));

    // step 2: Configure (deflate source)
    deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = (Bytef *)input;
    src.avail_in = (uInt)inputLen;
    src.next_out = comp;
    src.avail_out = (uInt)bound;
    deflate(&src, 0);
    unsigned int first_out_len = (unsigned int)src.total_out;

    // step 3: Operate (copy deflate state, continue deflate, then init inflate and decompress)
    deflateCopy(&dst, &src);
    dst.next_out = comp + first_out_len;
    dst.avail_out = (uInt)bound;
    deflate(&dst, 0);
    unsigned int total_comp_len = (unsigned int)dst.total_out;
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)total_comp_len;
    inf.next_out = (Bytef *)output;
    inf.avail_out = (uInt)sizeof(output);
    inflate(&inf, 0);
    unsigned int decomp_len = (unsigned int)inf.total_out;
    (void)decomp_len;

    // step 4: Validate & Cleanup
    gzFile gz = gzdopen(1, "wb");
    gzclose(gz);
    deflateEnd(&src);
    deflateEnd(&dst);
    inflateEnd(&inf);
    free(comp);

    // API sequence test completed successfully
    return 66;
}