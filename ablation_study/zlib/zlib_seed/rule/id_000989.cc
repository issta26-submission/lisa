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
//<ID> 989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'B', (size_t)ORIG_LEN);
    uLong comp_bound = compressBound(ORIG_LEN);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress data and write a byte to a gzip file
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gw, (int)orig[0]);
    gzclose_w(gw);

    // step 3: Initialize inflate, perform resets, inflate compressed data and compute crc
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
    inflateReset(&istrm);
    inflateResetKeep(&istrm);
    inflate(&istrm, 0);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    inflateEnd(&istrm);

    // step 4: Read back byte from gzip file, cleanup and finalize
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int read_c = gzgetc_(gr);
    gzclose(gr);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)crc_orig;
    (void)crc_decomp;
    (void)read_c;
    // API sequence test completed successfully
    return 66;
}