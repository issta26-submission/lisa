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
//<ID> 396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and compress it one-shot
    const char src[] = "zlib API sequence payload: prepare -> compress2 -> inflate -> gzwrite/gzread -> gzdopen -> zError";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)bound;
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate stream and perform a single-shot inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    Bytef *decomp = (Bytef *)malloc((size_t)sourceLen);
    memset(decomp, 0, (size_t)sourceLen);
    strm.next_out = decomp;
    strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&strm, 0);
    int rc_inflate_end = inflateEnd(&strm);

    // step 3: Write the inflated data into a .gz file, then read it back; also exercise gzdopen
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gw = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gw, decomp, (unsigned int)strm.total_out);
    int rc_gw_close = gzclose(gw);
    gzFile gr = gzopen(fname, "rb");
    Bytef *readback = (Bytef *)malloc((size_t)sourceLen);
    memset(readback, 0, (size_t)sourceLen);
    int rc_gzread = gzread(gr, readback, (unsigned int)sourceLen);
    int rc_gr_close = gzclose(gr);
    gzFile gzd = gzdopen(1, "wb");
    int rc_gzdwrite = gzwrite(gzd, (voidpc)readback, (unsigned int)rc_gzread);
    int rc_gzdclose = gzclose(gzd);

    // step 4: Finalize, query zlib error text, cleanup and return
    const char *errtext = zError(0);
    free(comp);
    free(decomp);
    free(readback);
    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)rc_gzwrite;
    (void)rc_gw_close;
    (void)rc_gzread;
    (void)rc_gr_close;
    (void)rc_gzdwrite;
    (void)rc_gzdclose;
    (void)errtext;
    // API sequence test completed successfully
    return 66;
}