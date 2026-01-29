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
//<ID> 988
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Write a few bytes to a gzip file using gzputc, then read one byte with gzgetc_
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gw, (int)orig[0]);
    gzputc(gw, (int)orig[1]);
    gzputc(gw, (int)orig[2]);
    gzclose_w(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int read_ch_int = gzgetc_(gr);
    unsigned char read_ch = (unsigned char)read_ch_int;
    gzclose(gr);
    uLong crc_read_byte = crc32(0L, (const Bytef *)&read_ch, (uInt)1);

    // step 3: Initialize inflate stream, copy it, reset the copy and keep the original, then inflate both
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
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm);
    inflate(&istrm, 0);
    inflateReset(&istrm_copy);
    istrm_copy.next_in = comp;
    istrm_copy.avail_in = (uInt)comp_len;
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);
    istrm_copy.next_out = decomp2;
    istrm_copy.avail_out = (uInt)ORIG_LEN;
    inflateResetKeep(&istrm);
    inflate(&istrm_copy, 0);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc_decomp2 = crc32(0L, decomp2, (uInt)ORIG_LEN);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);

    // step 4: Cleanup and finalize
    free(orig);
    free(comp);
    free(decomp);
    free(decomp2);
    (void)ver;
    (void)crc_orig;
    (void)crc_read_byte;
    (void)crc_decomp;
    (void)crc_decomp2;
    // API sequence test completed successfully
    return 66;
}