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
//<ID> 476
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and perform a one-shot compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit_ -> inflate -> deflateInit_ -> deflatePending -> gz I/O -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream, inflate compressed data into dest buffer, then end inflate
    z_stream inf;
    memset(&inf, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)compLen;
    uLong destCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)destCap);
    memset(dest, 0, (size_t)destCap);
    inf.next_out = dest;
    inf.avail_out = (uInt)destCap;
    int rc_inflate = inflate(&inf, 0);
    uLong decompressed = inf.total_out;
    int rc_inf_end = inflateEnd(&inf);

    // step 3: Initialize deflate stream, compress the decompressed data, query pending bits, and end deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = dest;
    dstrm.avail_in = (uInt)decompressed;
    uLong outBound = deflateBound(&dstrm, (uLong)dstrm.avail_in);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write deflated output to a gzFile, check direct mode, close, and cleanup
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    z_size_t written = gzfwrite((voidpc)outBuf, (z_size_t)1, (z_size_t)dstrm.total_out, gf);
    int is_direct = gzdirect(gf);
    int rc_gzclose = gzclose(gf);

    free(comp);
    free(dest);
    free(outBuf);
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_pending;
    (void)rc_def_end;
    (void)written;
    (void)is_direct;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}