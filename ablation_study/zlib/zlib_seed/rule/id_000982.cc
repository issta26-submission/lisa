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
//<ID> 982
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 32UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data and write a small marker to a gzip file
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gw, 'M');
    gzputc(gw, 'K');
    gzputc(gw, '1');
    gzclose_w(gw);

    // step 3: Initialize inflate, reset, inflate, reset keep, inflate again, compute CRCs
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset(&istrm);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateResetKeep(&istrm);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);

    // step 4: Read marker bytes back with gzgetc_, cleanup and finalize
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int r1 = gzgetc_(gr);
    int r2 = gzgetc_(gr);
    int r3 = gzgetc_(gr);
    gzclose(gr);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    free(decomp2);
    (void)ver;
    (void)crc_orig;
    (void)crc_decomp;
    (void)r1;
    (void)r2;
    (void)r3;
    // API sequence test completed successfully
    return 66;
}