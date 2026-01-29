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
//<ID> 222
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
    memset(input, 'X', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);

    uLong compBound1 = compressBound(inputLen);
    Bytef *compBuf1 = (Bytef *)malloc((size_t)compBound1);
    uLong compLen1 = compBound1;
    compress2(compBuf1, &compLen1, (const Bytef *)input, inputLen, 6);

    // step 2: Setup inflation and basic decompression
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    inf_strm.next_in = compBuf1;
    inf_strm.avail_in = (uInt)compLen1;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)inputLen;
    inflate(&inf_strm, 0);
    inflateUndermine(&inf_strm, 1);

    // step 3: Setup deflation, recompress decompressed data, and reset keeper
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong recompressBound = deflateBound(&def_strm, (uLong)inf_strm.total_out);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = outBuf;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);
    deflateResetKeep(&def_strm);

    // step 4: Validate & Cleanup
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf1);
    free(outBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}