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
//<ID> 940
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and prepare output buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;

    // step 3: Operate: inflate then finalize inflate usage
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Use gzFile to persist compressed data and exercise gzgetc/gzungetc/gztell, then cleanup
    const char *fname = "tmp_zlib_test.gz";
    gzFile gw = gzopen(fname, "wb");
    gzwrite(gw, comp, (unsigned int)comp_len);
    gzclose(gw);
    gzFile gr = gzopen(fname, "rb");
    int c = gzgetc(gr);
    gzungetc(c, gr);
    off_t pos = gztell(gr);
    gzclose(gr);

    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)pos;
    // API sequence test completed successfully
    return 66;
}