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
//<ID> 967
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
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and decompress
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

    // step 3: Copy inflate stream state, compute checksums, reset
    z_stream copystrm;
    memset(&copystrm, 0, sizeof(copystrm));
    copystrm.zalloc = NULL;
    copystrm.zfree = NULL;
    copystrm.opaque = NULL;
    inflateCopy(&copystrm, &istrm);
    uLong adl_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adl_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    off64_t len64 = (off64_t)ORIG_LEN;
    uLong combined = crc32_combine64(adl_orig, adl_decomp, len64);
    istrm.adler = adl_decomp;
    inflateReset(&istrm);

    // step 4: Cleanup and finish
    inflateEnd(&istrm);
    inflateEnd(&copystrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)comp_len;
    (void)adl_orig;
    (void)adl_decomp;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}