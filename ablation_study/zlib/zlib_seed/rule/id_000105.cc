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
    // step 1: Declarations & Initialize
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    unsigned char input[128];
    unsigned char decomp[128];
    memset(input, 'A', sizeof(input));
    memset(decomp, 0, sizeof(decomp));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    unsigned char *comp = (unsigned char *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Configure
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = (Bytef *)comp;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 4);
    unsigned int comp_len = (unsigned int)def_strm.total_out;

    // step 3: Operate & Validate
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = (Bytef *)comp;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = (Bytef *)decomp;
    inf_strm.avail_out = (uInt)sizeof(decomp);
    inflate(&inf_strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)comp, comp_len);
    gzflush(gz, 2);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(comp);

    // API sequence test completed successfully
    return 66;
}