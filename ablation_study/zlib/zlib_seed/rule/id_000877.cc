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
//<ID> 877
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
    uLong HALF = ORIG_LEN / 2;
    uLong crc1 = crc32(0UL, orig, (uInt)HALF);
    uLong crc2 = crc32(0UL, orig + HALF, (uInt)HALF);
    uLong crc_combined = crc32_combine64(crc1, crc2, (off64_t)HALF);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *comp2 = (Bytef *)malloc((size_t)bound);
    uLongf comp2Len = (uLongf)bound;
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;

    // step 2: Initialize & configure (compress and deflate)
    compress2(comp, &compLen, orig, ORIG_LEN, 6);
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong produced = dstrm.total_out;
    comp2Len = (uLongf)produced;

    // step 3: Operate & validate (inflate, sync check, write via gzdopen)
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp2;
    istrm.avail_in = (uInt)comp2Len;
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateSyncPoint(&istrm);
    gzFile gf = gzdopen(1, "wb");
    gzwrite(gf, outbuf, (unsigned int)ORIG_LEN);

    // step 4: Cleanup
    gzclose(gf);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    free(comp2);
    free(outbuf);
    (void)ver;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)compLen;
    (void)comp2Len;
    (void)bound;
    (void)produced;
    // API sequence test completed successfully
    return 66;
}