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
//<ID> 477
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and initialize deflate stream
    const char src[] = "zlib API sequence payload: deflate -> deflatePending -> inflate -> write gz -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    // step 2: Operate deflate and query pending bits
    int rc_def = deflate(&dstrm, 4);
    unsigned int pending = 0;
    int bits = 0;
    int rc_def_pending = deflatePending(&dstrm, &pending, &bits);
    uLong compLen = dstrm.total_out;

    // step 3: Initialize inflate, perform inflation, and finalize inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLong outCap = sourceLen * 2;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    istrm.next_out = dest;
    istrm.avail_out = (uInt)outCap;
    int rc_inflate = inflate(&istrm, 0);
    uLong produced = istrm.total_out;
    int rc_inf_end = inflateEnd(&istrm);

    // step 4: Write inflated output to gzFile, check direct mode, close, cleanup deflate and memory
    gzFile gf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)dest, (unsigned int)produced);
    int rc_gzdirect = gzdirect(gf);
    int rc_gzclose = gzclose(gf);
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(dest);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_pending;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzdirect;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}