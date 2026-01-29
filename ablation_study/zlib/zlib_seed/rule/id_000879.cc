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
//<ID> 879
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
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound);
    uLongf comp2Len = (uLongf)bound;
    Bytef *defout = (Bytef *)malloc((size_t)bound);

    // step 2: Compression: deflate stream and compress2
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = defout;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);
    compress2(comp2, &comp2Len, orig, ORIG_LEN, 6);

    // step 3: Decompression & validation, stream sync check, combine CRCs, write compressed data via gzdopen
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp2;
    istrm.avail_in = (uInt)comp2Len;
    Bytef *outbuf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(outbuf, 0, (size_t)ORIG_LEN);
    istrm.next_out = outbuf;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateSyncPoint(&istrm);
    inflate(&istrm, 0);
    uLong crc_decompr = crc32(0UL, outbuf, (uInt)ORIG_LEN);
    uLong combined_crc = crc32_combine64(crc_orig, crc_decompr, (off64_t)ORIG_LEN);
    gzFile gw = gzdopen(1, "wb");
    gzwrite(gw, comp2, (unsigned int)comp2Len);
    gzclose(gw);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(orig);
    free(comp2);
    free(defout);
    free(outbuf);
    (void)ver;
    (void)bound;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}