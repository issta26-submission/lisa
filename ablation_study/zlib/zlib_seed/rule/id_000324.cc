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
//<ID> 324
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
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure compression buffers and stream pointers
    uLong bound = deflateBound(&def_strm, inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate & Validate (compress -> prepare inflate -> validate/prime/mark -> inflate)
    deflate(&def_strm, 4);
    uInt compLen = (uInt)def_strm.total_out;
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = compLen;
    Bytef * outBuf = (Bytef *)malloc(512);
    memset(outBuf, 0, 512);
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = 512;
    inflateValidate(&inf_strm, 1);
    inflatePrime(&inf_strm, 1, 1);
    long mark = inflateMark(&inf_strm);
    (void)mark;
    inflate(&inf_strm, 4);

    // step 4: Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(outBuf);
    return 66;
    // API sequence test completed successfully
}