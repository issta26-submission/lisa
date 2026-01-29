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
//<ID> 176
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
    unsigned char plain[128];
    unsigned char midout[256];
    memset(plain, 'X', sizeof(plain));
    memset(midout, 0, sizeof(midout));
    uLong plainLen = (uLong)sizeof(plain);
    uLong compBound = compressBound(plainLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)plain, plainLen, 6);

    // step 2: Setup (initialize inflater and deflater, apply reset-keep)
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);

    // step 3: Operate (inflate with priming and sync check, then deflate the result)
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = (Bytef *)midout;
    inf_strm.avail_out = (uInt)sizeof(midout);
    inflatePrime(&inf_strm, 2, 1);
    inflate(&inf_strm, 0);
    inflateSyncPoint(&inf_strm);

    uLong recompressBound = compressBound((uLong)inf_strm.total_out);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_in = (Bytef *)midout;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);
    gzrewind(gz);
    gzclose(gz);
    deflateEnd(&def_strm);
    inflateEnd(&inf_strm);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}