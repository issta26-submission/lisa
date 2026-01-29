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
//<ID> 286
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
    z_stream inf_copy_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&inf_copy_strm, 0, sizeof(inf_copy_strm));
    unsigned char input[128];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Operate (compress)
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 4);
    uLong compLen = def_strm.total_out;
    deflateReset(&def_strm);

    // step 3: Prepare inflate, copy stream state and inflate both
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    unsigned char outBuf1[256];
    unsigned char outBuf2[256];
    memset(outBuf1, 0, sizeof(outBuf1));
    memset(outBuf2, 0, sizeof(outBuf2));
    inf_strm.next_out = outBuf1;
    inf_strm.avail_out = (uInt)sizeof(outBuf1);
    inflateCopy(&inf_copy_strm, &inf_strm);
    inf_copy_strm.next_out = outBuf2;
    inf_copy_strm.avail_out = (uInt)sizeof(outBuf2);
    inflate(&inf_strm, 0);
    inflate(&inf_copy_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, "gz output header\n");
    gzwrite(gz, outBuf1, (unsigned int)inf_strm.total_out);
    gzwrite(gz, outBuf2, (unsigned int)inf_copy_strm.total_out);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    inflateEnd(&inf_copy_strm);
    free(compBuf);

    return 66;
    // API sequence test completed successfully
}