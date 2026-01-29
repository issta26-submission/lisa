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
//<ID> 148
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
    unsigned char output[256];
    memset(input, 'A', sizeof(input));
    memset(output, 0, sizeof(output));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBound = compressBound(inputLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);

    // step 2: Setup (initialize deflate with custom params, prepare inflate)
    deflateInit2_(&def_strm, 6, 8, 15, 8, 0, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = comp;
    def_strm.avail_out = (uInt)compBound;

    // step 3: Operate (deflate -> reset-keep -> deflate finish -> inflate with undermine -> write gz & get position)
    deflate(&def_strm, 0);
    deflateResetKeep(&def_strm);
    deflate(&def_strm, 4);
    inf_strm.next_in = comp;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = (Bytef *)output;
    inf_strm.avail_out = (uInt)sizeof(output);
    inflateUndermine(&inf_strm, 1);
    inflate(&inf_strm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)output, (unsigned int)inf_strm.total_out);
    off64_t pos = gztell64(gz);
    gzclose(gz);

    // step 4: Validate & Cleanup
    uLong checksum = crc32(0L, output, (uInt)inf_strm.total_out);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(comp);
    (void)pos;
    (void)checksum;
    return 66;
    // API sequence test completed successfully
}