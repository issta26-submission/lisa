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
//<ID> 99
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
    z_stream inf_src;
    z_stream inf_copy;
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf_src, 0, sizeof(inf_src));
    memset(&inf_copy, 0, sizeof(inf_copy));
    unsigned char input[128];
    unsigned char decomp[128];
    memset(input, 'X', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    def_src.next_in = (Bytef *)input;
    def_src.avail_in = (uInt)inputLen;
    def_src.next_out = (Bytef *)comp;
    def_src.avail_out = (uInt)bound;
    deflate(&def_src, 0);
    deflateCopy(&def_copy, &def_src);
    unsigned int comp_len = (unsigned int)def_src.total_out;
    inflateInit_(&inf_src, zlibVersion(), (int)sizeof(z_stream));
    inf_src.next_in = (Bytef *)comp;
    inf_src.avail_in = (uInt)comp_len;
    inf_src.next_out = (Bytef *)decomp;
    inf_src.avail_out = (uInt)sizeof(decomp);
    inflateCopy(&inf_copy, &inf_src);

    // step 3: Operate & Validate
    inflateValidate(&inf_src, 1);
    inflateUndermine(&inf_src, 0);
    inflateBackEnd(&inf_src);

    // step 4: Cleanup
    inflateEnd(&inf_src);
    inflateEnd(&inf_copy);
    deflateEnd(&def_src);
    deflateEnd(&def_copy);
    free(comp);

    // API sequence test completed successfully
    return 66;
}