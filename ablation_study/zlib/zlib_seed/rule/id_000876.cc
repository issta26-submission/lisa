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
//<ID> 876
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
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress2(comp, &compLen, orig, ORIG_LEN, 6);

    // step 2: Initialize & run a deflate stream (separate from compress2 usage)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    Bytef *defout = (Bytef *)malloc((size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = defout;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);

    // step 3: Inflate compressed data, validate with inflateSyncPoint, combine CRCs, and write via gzdopen
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 4);
    int sync = inflateSyncPoint(&istrm);
    uLong crc_decomp = crc32(0UL, outbuf, (uInt)ORIG_LEN);
    uLong crc_combined = crc32_combine64(crc_orig, crc_decomp, (off64_t)ORIG_LEN);
    gzFile gw = gzdopen(1, "wb");
    gzwrite(gw, outbuf, (unsigned int)ORIG_LEN);
    gzclose(gw);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(defout);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)compLen;
    (void)sync;
    (void)crc_orig;
    (void)crc_decomp;
    (void)crc_combined;
    return 66;
}