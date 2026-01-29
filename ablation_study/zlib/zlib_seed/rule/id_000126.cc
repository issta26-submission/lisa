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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def;
    z_stream def_copy;
    z_stream inf;
    memset(&def, 0, sizeof(def));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf, 0, sizeof(inf));
    unsigned char input[128];
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Setup (deflate init, compress, and copy)
    deflateInit_(&def, 6, zlibVersion(), (int)sizeof(z_stream));
    def.next_in = (Bytef *)input;
    def.avail_in = (uInt)inputLen;
    def.next_out = comp;
    def.avail_out = (uInt)bound;
    deflate(&def, 0);
    unsigned int comp_len = (unsigned int)def.total_out;
    deflateCopy(&def_copy, &def);

    // step 3: Operate (inflate and write via gzdopen)
    inflateInit_(&inf, zlibVersion(), (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = (Bytef *)output;
    inf.avail_out = (uInt)sizeof(output);
    inflate(&inf, 0);
    unsigned int decomp_len = (unsigned int)inf.total_out;
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)decomp_len);

    // step 4: Validate & Cleanup
    deflateEnd(&def);
    deflateEnd(&def_copy);
    inflateEnd(&inf);
    gzclose(gz);
    free(comp);

    // API sequence test completed successfully
    return 66;
}