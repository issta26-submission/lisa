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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream inf_strm;
    z_stream def_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(&def_strm, 0, sizeof(def_strm));
    unsigned char input[512];
    memset(input, 'G', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    // step 2: Prepare a zlib-compressed buffer from the raw input
    uLong compBoundInitial = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBoundInitial);
    uLong compLen = compBoundInitial;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 3: Setup streams and perform inflate then deflate operations
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    inflateUndermine(&inf_strm, 1);

    Bytef *midBuf = (Bytef *)malloc((size_t)inputLen);
    memset(midBuf, 0, (size_t)inputLen);
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = midBuf;
    inf_strm.avail_out = (uInt)inputLen;
    inflate(&inf_strm, 0);

    uLong recompressBound = deflateBound(&def_strm, (uLong)inf_strm.total_out);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = midBuf;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);
    deflateResetKeep(&def_strm);
    deflate(&def_strm, 0);

    // step 4: Validate & Cleanup
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(midBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}