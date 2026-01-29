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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and perform one-shot compression into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit_ -> inflate -> deflateInit_ -> deflatePending -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize inflate stream, inflate the compressed buffer, then end the inflate stream
    z_stream strm_infl;
    memset(&strm_infl, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm_infl, version, (int)sizeof(z_stream));
    strm_infl.next_in = comp;
    strm_infl.avail_in = (uInt)compLen;
    uLong destCap = sourceLen * 2 + 16;
    Bytef *dest = (Bytef *)malloc((size_t)destCap);
    memset(dest, 0, (size_t)destCap);
    strm_infl.next_out = dest;
    strm_infl.avail_out = (uInt)destCap;
    int rc_inflate = inflate(&strm_infl, 0);
    uLong out_size = strm_infl.total_out;
    int rc_inf_end = inflateEnd(&strm_infl);

    // step 3: Initialize deflate stream, deflate the inflated data, query pending bytes/bits, then end deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = dest;
    dstrm.avail_in = (uInt)out_size;
    uLong outBound = deflateBound(&dstrm, out_size);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);
    unsigned int pending = 0;
    int bits = 0;
    int rc_pending = deflatePending(&dstrm, &pending, &bits);
    uLong deflated_size = dstrm.total_out;
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Write deflated output to a gzFile, check direct mode, close and cleanup
    gzFile gf = gzopen("zlib_seq_output.gz", "wb");
    int rc_write = gzwrite(gf, (voidpc)outBuf, (unsigned int)deflated_size);
    int rc_direct = gzdirect(gf);
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
    (void)rc_write;
    (void)rc_direct;
    (void)rc_gzclose;
    (void)version;
    (void)pending;
    (void)bits;
    // API sequence test completed successfully
    return 66;
}