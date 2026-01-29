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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_src;
    z_stream def_copy;
    z_stream inf;
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf, 0, sizeof(inf));
    unsigned char input[256];
    unsigned char output[512];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure (deflate)
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    def_src.next_in = (Bytef *)input;
    def_src.avail_in = (uInt)inputLen;
    def_src.next_out = comp;
    def_src.avail_out = (uInt)bound;
    deflate(&def_src, 0);

    // step 3: Operate (copy deflate state, continue, inflate, write gz)
    deflateCopy(&def_copy, &def_src);
    def_copy.next_out = comp + (uInt)def_src.total_out;
    def_copy.avail_out = (uInt)(bound - (uLong)def_src.total_out);
    deflate(&def_copy, 0);
    uLong comp_len_total = def_copy.total_out;
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)comp_len_total;
    inf.next_out = (Bytef *)output;
    inf.avail_out = (uInt)sizeof(output);
    inflate(&inf, 0);
    unsigned int decomp_len = (unsigned int)inf.total_out;
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, (voidpc)output, decomp_len);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def_src);
    deflateEnd(&def_copy);
    inflateEnd(&inf);
    free(comp);

    // API sequence test completed successfully
    return 66;
}