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
//<ID> 470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress with compress()
    const char src[] = "zlib API sequence payload: prepare -> compress -> deflate -> deflatePending -> inflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize deflate stream, run deflate, query pending bits, capture output length
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = comp;
    dstrm.avail_in = (uInt)compLen;
    uLong outBound = deflateBound(&dstrm, compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    uLong compressed_len = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, inflate the deflated data, then end inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = outBuf;
    istrm.avail_in = (uInt)compressed_len;
    uLong outCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Write decompressed data to a gzFile, check direct mode, close, and cleanup
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    z_size_t written = gzfwrite((voidpc)dest, (z_size_t)1, (z_size_t)istrm.total_out, gf);
    int direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);
    free(comp);
    free(outBuf);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_pending;
    (void)pending;
    (void)bits;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)written;
    (void)direct;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}