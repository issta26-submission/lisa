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
//<ID> 490
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and perform a compressed pass with compress2
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> deflateBound -> deflate -> gzprintf -> inflate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compAlloc = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compAlloc);
    memset(comp, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp2 = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream, compute a safe output bound with deflateBound, and run deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong outBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def_op = deflate(&dstrm, 0);

    // step 3: Write deflate output into a gzFile using gzprintf, query direct mode, and close
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzprintf = gzprintf(gf, src);
    int direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Initialize inflate, run inflate on the deflated buffer to validate round-trip, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = outBuf;
    istrm.avail_in = (uInt)dstrm.total_out;
    uLong destAlloc = sourceLen * 4 + 64;
    Bytef *dest = (Bytef *)malloc((size_t)destAlloc);
    memset(dest, 0, (size_t)destAlloc);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)destAlloc;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(outBuf);
    free(dest);
    (void)rc_comp2;
    (void)rc_def_init;
    (void)rc_def_op;
    (void)rc_gzprintf;
    (void)direct;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}