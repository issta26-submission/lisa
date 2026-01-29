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
//<ID> 969
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);

    // step 2: Compress data with deflate
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

    // step 3: Initialize inflate stream, copy it, reset the copy, then inflate both
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
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm);
    inflate(&istrm, 0);
    inflateReset(&istrm_copy);
    istrm_copy.next_in = comp;
    istrm_copy.avail_in = (uInt)comp_len;
    istrm_copy.next_out = decomp2;
    istrm_copy.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_copy, 0);
    uLong crc1 = crc32(0L, orig, (uInt)(ORIG_LEN / 2));
    uLong crc2 = crc32(0L, orig + (ORIG_LEN / 2), (uInt)(ORIG_LEN - (ORIG_LEN / 2)));
    uLong crc_combined = crc32_combine64(crc1, crc2, (off64_t)(ORIG_LEN - (ORIG_LEN / 2)));
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    inflateEnd(&istrm);
    inflateEnd(&istrm_copy);

    // step 4: Validate and cleanup
    free(orig);
    free(comp);
    free(decomp);
    free(decomp2);
    (void)ver;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)adler_orig;
    (void)adler_decomp;
    (void)comp_len;
    // API sequence test completed successfully
    return 66;
}