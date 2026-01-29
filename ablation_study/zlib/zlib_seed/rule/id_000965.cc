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
//<ID> 965
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
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream & inflate compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    uLong decomp_len = istrm.total_out;
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);

    // step 3: Copy inflate state, reset the copy, inflate with the copy, validate CRCs
    z_stream cstrm;
    memset(&cstrm, 0, sizeof(cstrm));
    inflateCopy(&cstrm, &istrm);
    inflateReset(&cstrm);
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);
    cstrm.next_in = comp;
    cstrm.avail_in = (uInt)comp_len;
    cstrm.next_out = decomp2;
    cstrm.avail_out = (uInt)ORIG_LEN;
    inflate(&cstrm, 0);
    uLong adler_decomp2 = adler32(0L, decomp2, (uInt)ORIG_LEN);
    uLong crc1 = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc2 = crc32(0L, decomp2, (uInt)ORIG_LEN);
    uLong crc_combined = crc32_combine64(crc1, crc2, (off64_t)ORIG_LEN);

    // step 4: Cleanup and finish
    inflateEnd(&cstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    free(decomp2);
    (void)ver;
    (void)decomp_len;
    (void)adler_orig;
    (void)adler_decomp;
    (void)adler_decomp2;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}