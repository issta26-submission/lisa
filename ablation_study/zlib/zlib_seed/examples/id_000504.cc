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
//<ID> 504
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and allocate buffers using compressBound
    const char src[] = "zlib API sequence payload: initialize -> configure -> operate -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong outBound = compressBound(sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);

    // step 2: Initialize and configure deflate stream, then adjust parameters with deflateParams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def_params = deflateParams(&dstrm, 5, 0);

    // step 3: Operate deflate, query pending bits with deflatePending, then write to gzFile and check direct I/O
    int rc_def_op = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int pending_bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &pending_bits);
    gzFile gf = gzopen("test_zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    int rc_gzflush = gzflush(gf, 0);

    // step 4: Cleanup streams and memory
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(outBuf);
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_def_op;
    (void)rc_pending;
    (void)pending;
    (void)pending_bits;
    (void)rc_gzwrite;
    (void)direct;
    (void)rc_gzflush;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}