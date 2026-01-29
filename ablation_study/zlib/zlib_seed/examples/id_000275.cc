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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compute adler32
    const char src_c[] = "zlib api sequence payload: compute adler32_z -> init streams -> deflate/inflate -> cleanup";
    const Bytef *source = (const Bytef *)src_c;
    z_size_t src_zlen = (z_size_t)(sizeof(src_c) - 1);
    uLong sourceLen = (uLong)src_zlen;
    uLong adler_initial = adler32_z(0UL, source, src_zlen);

    // step 2: Initialize and configure deflate and inflate streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    dstrm.adler = adler_initial;
    istrm.adler = adler_initial;

    // step 3: Operate - compute bounds, perform a simple deflate/inflate pass
    uLong bound_by_compressBound = compressBound(sourceLen);
    uLong bound_by_deflateBound = deflateBound(&dstrm, sourceLen);
    uLong compBufSize = (bound_by_compressBound > bound_by_deflateBound) ? bound_by_compressBound : bound_by_deflateBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBufSize);
    memset(compBuf, 0, (size_t)compBufSize);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf;
    dstrm.avail_out = (uInt)compBufSize;
    int rc_deflate = deflate(&dstrm, 0);
    z_size_t produced_comp = (z_size_t)((dstrm.total_out <= compBufSize) ? dstrm.total_out : compBufSize);

    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 64));
    memset(decompBuf, 0, (size_t)(sourceLen + 64));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)produced_comp;
    istrm.next_out = decompBuf;
    istrm.avail_out = (uInt)(sourceLen + 64);
    int rc_inflate = inflate(&istrm, 0);
    z_size_t produced_decomp = (z_size_t)((istrm.total_out <= (sourceLen + 64)) ? istrm.total_out : (sourceLen + 64));
    uLong adler_after = adler32_z(0UL, decompBuf, (z_size_t)produced_decomp);

    // step 4: Validate-like propagation and cleanup
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_def_init;
    (void)rc_inf_init;
    (void)rc_deflate;
    (void)rc_inflate;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)bound_by_compressBound;
    (void)bound_by_deflateBound;
    (void)compBufSize;
    (void)produced_comp;
    (void)produced_decomp;
    (void)adler_initial;
    (void)adler_after;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}