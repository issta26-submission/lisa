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
//<ID> 955
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 400UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'M', (size_t)ORIG_LEN);
    uLongf comp_len = 0;
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);
    uLong adler_before = adler32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Initialize deflate stream and exercise deflateResetKeep
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    int rc_def_init = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    int rc_def_resetkeep = deflateResetKeep(&dstrm);

    // step 3: Initialize inflate stream, reset it, inflate compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int rc_inf_init = inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    int rc_inf_reset = inflateReset(&istrm);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Validate, use gzFile, cleanup
    uLong adler_after = adler32(0L, decomp, (uInt)ORIG_LEN);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwritten = gzwrite(gf, comp, (unsigned int)comp_len);
    int gclose_rc = gzclose(gf);
    int rc_def_end = deflateEnd(&dstrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)rc_def_init;
    (void)rc_def_resetkeep;
    (void)rc_inf_init;
    (void)rc_inf_reset;
    (void)rc_inflate;
    (void)rc_inflate_end;
    (void)adler_before;
    (void)adler_after;
    (void)gwritten;
    (void)gclose_rc;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}