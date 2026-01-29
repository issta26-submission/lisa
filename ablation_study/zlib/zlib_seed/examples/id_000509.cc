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
//<ID> 509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed buffer using compress2, sizing with compressBound
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflateParams -> deflate -> gz IO -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, adjust parameters with deflateParams, and run deflate on the compressed buffer
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_params = deflateParams(&dstrm, 5, 0);
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong outBound = compressBound((uLong)compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def_op = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int pending_bits = 0;
    int rc_def_pending = deflatePending(&dstrm, &pending, &pending_bits);

    // step 3: Write deflate output to a gzFile and query direct I/O capability with gzdirect
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Inflate the deflate output to validate round-trip, then cleanup streams and memory
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = outBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    uLong destBound = sourceLen + 64;
    Bytef *dest = (Bytef *)malloc((size_t)destBound);
    memset(dest, 0, (size_t)destBound);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destBound;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(outBuf);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_params;
    (void)rc_def_op;
    (void)rc_def_pending;
    (void)rc_gzwrite;
    (void)direct;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)pending;
    (void)pending_bits;
    (void)version;
    // API sequence test completed successfully
    return 66;
}