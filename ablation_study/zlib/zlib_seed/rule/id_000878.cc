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
//<ID> 878
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
    uLong crc_initial = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup: compress with compress2 and perform a streaming deflate
    compress2(comp, &compLen, orig, ORIG_LEN, 6);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong scBound = compressBound(ORIG_LEN);
    Bytef *streamComp = (Bytef *)malloc((size_t)scBound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = streamComp;
    dstrm.avail_out = (uInt)scBound;
    deflate(&dstrm, 4);

    // step 3: Operate & validate: inflate the compressed blob, check sync point, combine CRCs, write via gzdopen
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
    uLong crc_combined = crc32_combine64(crc_initial, crc_initial, (off64_t)ORIG_LEN);
    gzFile gz = gzdopen(1, "wb");
    gzwrite(gz, outbuf, (unsigned int)ORIG_LEN);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    free(streamComp);
    free(outbuf);
    (void)ver;
    (void)crc_initial;
    (void)bound;
    (void)compLen;
    (void)sync;
    (void)crc_combined;
    (void)scBound;
    // API sequence test completed successfully
    return 66;
}