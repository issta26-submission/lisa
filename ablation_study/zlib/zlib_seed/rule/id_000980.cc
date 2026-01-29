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
//<ID> 980
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);

    uLong crc_first = crc32(0L, orig, (uInt)(ORIG_LEN / 2));
    uLong crc_total = crc32(crc_first, orig + (ORIG_LEN / 2), (uInt)(ORIG_LEN - (ORIG_LEN / 2)));

    // step 2: Compress data with deflate and write a few bytes to a gzip file
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 0);
    uLong comp_len = dstrm.total_out;
    deflateEnd(&dstrm);

    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gw, 'X');
    gzputc(gw, 'Y');
    gzputc(gw, 'Z');
    gzclose_w(gw);

    // step 3: Initialize inflate, perform inflate, reset and reuse stream then inflate again
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateReset(&istrm);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateResetKeep(&istrm);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Read back from gzip file using internal getter, cleanup and validate state
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int c1 = gzgetc_(gr);
    int c2 = gzgetc_(gr);
    int c3 = gzgetc_(gr);
    gzclose(gr);

    free(orig);
    free(comp);
    free(decomp);
    free(decomp2);

    (void)ver;
    (void)crc_first;
    (void)crc_total;
    (void)comp_len;
    (void)c1;
    (void)c2;
    (void)c3;
    // API sequence test completed successfully
    return 66;
}