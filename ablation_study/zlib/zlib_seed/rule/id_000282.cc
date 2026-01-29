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
//<ID> 282
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
    z_stream inf_copy;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&inf_copy, 0, sizeof(inf_copy));
    unsigned char inBuf[64];
    memset(inBuf, 'Z', sizeof(inBuf));
    uLong inLen = (uLong)sizeof(inBuf);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure & Operate (compress then reset)
    uLong bound = deflateBound(&def_strm, inLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)inBuf;
    def_strm.avail_in = (uInt)inLen;
    def_strm.next_out = comp;
    def_strm.avail_out = (uInt)bound;
    deflate(&def_strm, 4);
    deflateReset(&def_strm);
    inf_strm.next_in = comp;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    unsigned char outBuf1[128];
    unsigned char outBuf2[128];
    memset(outBuf1, 0, sizeof(outBuf1));
    memset(outBuf2, 0, sizeof(outBuf2));
    inf_strm.next_out = outBuf1;
    inf_strm.avail_out = (uInt)sizeof(outBuf1);

    // step 3: Validate (copy inflate state, inflate from both streams, write via gzputs)
    inflateCopy(&inf_copy, &inf_strm);
    inf_copy.next_out = outBuf2;
    inf_copy.avail_out = (uInt)sizeof(outBuf2);
    inflate(&inf_copy, 0);
    inflate(&inf_strm, 0);
    outBuf1[(size_t)inf_strm.total_out] = 0;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gz, (const char *)outBuf1);
    gzclose(gz);

    // step 4: Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    inflateEnd(&inf_copy);
    free(comp);

    return 66;
    // API sequence test completed successfully
}