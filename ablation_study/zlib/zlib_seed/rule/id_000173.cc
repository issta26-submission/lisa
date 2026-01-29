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
//<ID> 173
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
    unsigned char src[128];
    unsigned char mid[256];
    memset(src, 'A', sizeof(src));
    memset(mid, 0, sizeof(mid));
    uLong srcLen = (uLong)sizeof(src);
    uLong compBound = compressBound(srcLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, &compLen, (const Bytef *)src, srcLen, 6);

    // step 2: Setup (initialize inflater and deflater, prime and reset-keep)
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inflatePrime(&inf_strm, 1, 0);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflateResetKeep(&def_strm);

    // step 3: Operate (inflate compressed data, check sync point, then deflate result and write to gz)
    inf_strm.next_in = compBuf;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = (Bytef *)mid;
    inf_strm.avail_out = (uInt)sizeof(mid);
    inflate(&inf_strm, 0);
    inflateSyncPoint(&inf_strm);

    def_strm.next_in = (Bytef *)mid;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    uLong recompressBound = deflateBound(&def_strm, (uLong)def_strm.avail_in);
    Bytef *recompBuf = (Bytef *)malloc((size_t)recompressBound);
    memset(recompBuf, 0, (size_t)recompressBound);
    def_strm.next_out = recompBuf;
    def_strm.avail_out = (uInt)recompressBound;
    deflate(&def_strm, 0);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, (voidpc)recompBuf, (unsigned int)def_strm.total_out);
    gzclose(gz);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0L, mid, (uInt)inf_strm.total_out);
    (void)checksum;
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    free(compBuf);
    free(recompBuf);

    // API sequence test completed successfully
    return 66;
}