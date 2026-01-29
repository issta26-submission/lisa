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
//<ID> 474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit_ -> inflate -> deflateInit_ -> deflatePending -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize an inflate stream and perform inflation of the compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&istrm, 0);
    uLong inflated = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);

    // step 3: Initialize a deflate stream, compress the inflated data, and query pending bits
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = dest;
    dstrm.avail_in = (uInt)inflated;
    uLong outBound = deflateBound(&dstrm, (uLong)inflated);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_deflate = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write inflated data to a gzFile, query direct mode, close and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzdirect = gzdirect(gf);
    int rc_gzwrite = gzwrite(gf, (voidpc)dest, (unsigned int)inflated);
    int rc_gzclose = gzclose(gf);

    free(comp);
    free(dest);
    free(outBuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_init;
    (void)rc_deflate;
    (void)rc_pending;
    (void)rc_def_end;
    (void)rc_gzdirect;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}