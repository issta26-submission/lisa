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
//<ID> 171
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
    unsigned char input[128];
    unsigned char mid_out[256];
    memset(input, 'A', sizeof(input));
    memset(mid_out, 0, sizeof(mid_out));
    uLong inputLen = (uLong)sizeof(input);
    uLong compBoundInit = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBoundInit);
    uLong compLen = compBoundInit;
    compress2(compBuf, &compLen, (const Bytef *)input, inputLen, 6);

    // step 2: Setup
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);
    inflatePrime(&inf_strm, 1, 1);

    // step 3: Core operations
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = mid_out;
    inf_strm.avail_out = (uInt)sizeof(mid_out);
    inflate(&inf_strm, 0);
    int sync_flag = inflateSyncPoint(&inf_strm);
    (void)sync_flag;
    uLong recompressBound = compressBound((uLong)inf_strm.total_out);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = mid_out;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}