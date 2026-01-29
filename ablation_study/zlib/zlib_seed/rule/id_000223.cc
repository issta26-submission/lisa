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
//<ID> 223
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

    // step 2: Setup
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong compBound = deflateBound(&def_strm, inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    deflateResetKeep(&def_strm);

    // step 3: Core operations
    def_strm.next_in = (Bytef *)input;
    def_strm.avail_in = (uInt)inputLen;
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)compBound;
    deflate(&def_strm, 0);
    uLong compSize = def_strm.total_out;

    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compSize;
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)inputLen;
    inflateUndermine(&inf_strm, 1);
    inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    return 66;
}