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
//<ID> 872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations & initialize buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong crc_part1 = crc32(0UL, orig, (uInt)(ORIG_LEN / 2));
    uLong crc_part2 = crc32(0UL, orig + (ORIG_LEN / 2), (uInt)(ORIG_LEN / 2));
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;

    // Step 2: Configure: compress data and initialize deflate stream
    compress2(comp, &compLen, orig, ORIG_LEN, 6);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    Bytef *defout = (Bytef *)malloc(128);
    dstrm.next_out = defout;
    dstrm.avail_out = 128;
    deflate(&dstrm, 4);

    // Step 3: Operate & Validate: inflate compressed data, check sync point, combine CRCs, use gzdopen
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    int sync = inflateSyncPoint(&istrm);
    uLong combined_crc = crc32_combine64(crc_part1, crc_part2, (off64_t)(ORIG_LEN / 2));
    gzFile gz = gzdopen(1, "wb");
    gzclose(gz);

    // Step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(defout);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)compLen;
    (void)sync;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}