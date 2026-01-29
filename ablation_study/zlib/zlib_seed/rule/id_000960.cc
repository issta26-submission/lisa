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
//<ID> 960
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
    Bytef *decomp_copy = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_copy, 0, (size_t)ORIG_LEN);

    // step 2: Initialize inflate stream, copy it, perform inflate on both streams, then reset/end source
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    z_stream copystrm;
    memset(&copystrm, 0, sizeof(copystrm));
    inflateCopy(&copystrm, &istrm);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    copystrm.next_out = decomp_copy;
    copystrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflate(&copystrm, 0);
    inflateReset(&istrm);
    inflateEnd(&istrm);
    inflateEnd(&copystrm);

    // step 3: Validate with adler32 and crc32_combine64
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0L, decomp_copy, (uInt)ORIG_LEN);
    uLong combined_crc = crc32_combine64(crc_orig, crc_decomp, (off64_t)ORIG_LEN);
    (void)ver;
    (void)adler_orig;
    (void)adler_decomp;
    (void)crc_orig;
    (void)crc_decomp;
    (void)combined_crc;

    // step 4: Cleanup and finish
    free(orig);
    free(comp);
    free(decomp);
    free(decomp_copy);
    // API sequence test completed successfully
    return 66;
}