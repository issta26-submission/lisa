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
//<ID> 144
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
    unsigned char input[256];
    unsigned char *comp;
    unsigned char *decomp;
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    comp = (Bytef *)malloc((size_t)bound);
    decomp = (Bytef *)malloc((size_t)512);
    memset(comp, 0, (size_t)bound);
    memset(decomp, 0, (size_t)512);

    // step 2: Setup (initialize deflater with custom params and inflater)
    deflateInit2_(&def_strm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = comp;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate (deflate, reset-keep, deflate again, then inflate with undermine)
    deflate(&def_strm, 0);
    deflateResetKeep(&def_strm);
    deflate(&def_strm, 0);
    inf_strm.next_in = comp;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = decomp;
    inf_strm.avail_out = (uInt)512;
    inflateUndermine(&inf_strm, 1);
    inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence_out.gz", "wb");
    gzwrite(gz, (voidpc)decomp, (unsigned int)inf_strm.total_out);
    off64_t pos = gztell64(gz);
    (void)pos;
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    gzclose(gz);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}