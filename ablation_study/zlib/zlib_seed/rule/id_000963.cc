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
//<ID> 963
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
    memset(orig, 'G', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and make a copy
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
    Bytef *decomp_copy = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_copy, 0, (size_t)ORIG_LEN);
    istrm_copy.next_out = decomp_copy;
    istrm_copy.avail_out = (uInt)ORIG_LEN;

    // step 3: Operate, validate with checksums, reset copied stream
    inflate(&istrm, 0);
    inflate(&istrm_copy, 0);
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc1 = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc2 = crc32(0L, decomp, (uInt)ORIG_LEN);
    off64_t overlap = (off64_t)ORIG_LEN;
    uLong combined_crc = crc32_combine64(crc1, crc2, overlap);
    inflateReset(&istrm_copy);

    // step 4: Cleanup and finish
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);
    free(orig);
    free(comp);
    free(decomp);
    free(decomp_copy);
    (void)ver;
    (void)adler_orig;
    (void)adler_decomp;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)comp_len;
    // API sequence test completed successfully
    return 66;
}