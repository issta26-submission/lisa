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
//<ID> 1030
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong adler_before = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    const char *ver = zlibVersion();

    // step 2: Initialize deflate, reset keep, perform deflate, and cleanup deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, perform inflate, compute adler of decompressed data, and cleanup inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    uLong adler_after = adler32(0L, decomp, (uInt)ORIG_LEN);
    inflateEnd(&istrm);

    // step 4: Write original to gzip, then read back using gzgetc/gzungetc and final cleanup
    gzFile gw = gzopen("zlib_api_seq_temp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 0);
    gzclose(gw);
    gzFile gr = gzopen("zlib_api_seq_temp.gz", "rb");
    int c = gzgetc(gr);
    gzungetc(c, gr);
    gzclose(gr);
    free(orig);
    free(comp);
    free(decomp);
    (void)adler_before;
    (void)adler_after;
    (void)comp_size;
    (void)bound;
    (void)ver;
    (void)c;
    // API sequence test completed successfully
    return 66;
}