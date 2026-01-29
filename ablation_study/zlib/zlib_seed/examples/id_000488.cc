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
//<ID> 488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize deflate stream, compute output bound
    const char src[] = "zlib API sequence payload: prepare -> deflateBound -> deflate -> gz IO -> adler combine -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 2: Operate deflate to produce compressed data
    int rc_def_op = deflate(&dstrm, 0);

    // step 3: Compute adler32 checksums and combine, then write compressed data to a gzFile and query direct/offset
    uLong adler_a = adler32(0L, source, (uInt)sourceLen);
    uLong adler_b = adler32(0L, outBuf, (uInt)dstrm.total_out);
    uLong combined = adler32_combine(adler_a, adler_b, (off_t)sourceLen);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    off64_t off64 = gzoffset64(gf);

    // step 4: Cleanup streams and memory
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def_op;
    (void)adler_a;
    (void)adler_b;
    (void)combined;
    (void)rc_gzwrite;
    (void)direct;
    (void)off64;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}