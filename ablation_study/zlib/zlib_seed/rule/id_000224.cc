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
//<ID> 224
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
    unsigned char input[512];
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Setup
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;

    // step 3: Core operations
    deflate(&def_strm, 0);
    uLong compLen = def_strm.total_out;
    deflateResetKeep(&def_strm);
    uLong compBound2 = deflateBound(&def_strm, compLen);
    Bytef *compBuf2 = (Bytef *)malloc((size_t)compBound2);
    memset(compBuf2, 0, (size_t)compBound2);
    def_strm.next_in = compBuf;
    def_strm.avail_in = (uInt)compLen;
    def_strm.next_out = compBuf2;
    def_strm.avail_out = (uInt)compBound2;
    deflate(&def_strm, 0);
    uLong compLen2 = def_strm.total_out;

    Bytef *outBuf = (Bytef *)malloc((size_t)(inputLen * 4));
    memset(outBuf, 0, (size_t)(inputLen * 4));
    inf_strm.next_in = compBuf2;
    inf_strm.avail_in = (uInt)compLen2;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)(inputLen * 4);
    inflate(&inf_strm, 0);
    inflateUndermine(&inf_strm, 1);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(compBuf2);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}