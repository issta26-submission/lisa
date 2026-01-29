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
//<ID> 507
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and allocate output buffer using compressBound
    const char src[] = "zlib API sequence payload: initialize -> configure -> operate -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong outBound = compressBound(sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);

    // step 2: Initialize and configure deflate stream, then adjust parameters
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 9, 0); // set higher compression level, default strategy

    // step 3: Operate: perform compression, query pending bits, write compressed output to gzFile and check direct mode
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_deflate = deflate(&dstrm, Z_FINISH);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    gzFile gf = gzopen("test_zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);

    // step 4: Cleanup: end deflate, close gzFile, free buffers and finalize
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_deflate;
    (void)rc_pending;
    (void)pending;
    (void)bits;
    (void)rc_gzwrite;
    (void)direct;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}