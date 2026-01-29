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
//<ID> 964
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Validate originals with adler32 and crc32
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);

    // step 3: Initialize inflate stream, inflate, copy, reset, and combine CRCs
    z_stream istrm_src;
    z_stream istrm_copy;
    memset(&istrm_src, 0, sizeof(istrm_src));
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    istrm_src.zalloc = NULL;
    istrm_src.zfree = NULL;
    istrm_src.opaque = NULL;
    inflateInit_(&istrm_src, ver, (int)sizeof(z_stream));
    istrm_src.next_in = comp;
    istrm_src.avail_in = (uInt)comp_len;
    istrm_src.next_out = decomp;
    istrm_src.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm_src, 0);
    inflateCopy(&istrm_copy, &istrm_src);
    inflateReset(&istrm_copy);
    uLong crc_decomp = crc32(0L, decomp, (uInt)ORIG_LEN);
    uLong combined_crc = crc32_combine64(crc_orig, crc_decomp, (off64_t)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);

    // step 4: Cleanup and finish
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm_src);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)adler_orig;
    (void)adler_decomp;
    (void)crc_orig;
    (void)crc_decomp;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}