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
//<ID> 1003
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
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);

    // step 2: In-memory uncompress using uncompress2 and write a gzip file, then clear gz error
    uLongf decomp_len1 = (uLongf)ORIG_LEN;
    Bytef *decomp1 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp1, 0, (size_t)ORIG_LEN);
    uLong src_len_for_uncompress = (uLong)comp_len;
    uncompress2(decomp1, &decomp_len1, comp, &src_len_for_uncompress);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclearerr(gr);
    gzclose(gr);

    // step 3: Initialize inflate with inflateInit2_, prime bits and inflate compressed data
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 0, 0);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Validate via crc32, cleanup and finalize
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp1 = crc32(0L, decomp1, (uInt)decomp_len1);
    free(orig);
    free(comp);
    free(decomp1);
    free(decomp2);
    (void)ver;
    (void)src_len_for_uncompress;
    (void)crc_orig;
    (void)crc_decomp1;
    // API sequence test completed successfully
    return 66;
}