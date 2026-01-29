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
//<ID> 966
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
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize inflate stream and perform inflation
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

    // step 3: Copy, reset, validate checksums, and end streams
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    istrm_copy.zalloc = NULL;
    istrm_copy.zfree = NULL;
    istrm_copy.opaque = NULL;
    inflateCopy(&istrm_copy, &istrm);
    inflateReset(&istrm_copy);
    uLong ad_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong ad_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc1 = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc2 = crc32(0L, decomp, (uInt)ORIG_LEN);
    off64_t len64 = (off64_t)ORIG_LEN;
    uLong crc_combined = crc32_combine64(crc1, crc2, len64);
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm);

    // step 4: Cleanup and finish
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)ad_orig;
    (void)ad_decomp;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}