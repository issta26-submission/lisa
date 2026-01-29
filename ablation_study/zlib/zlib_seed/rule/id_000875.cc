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
//<ID> 875
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
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup & compression (use compress2 and initialize a deflate stream)
    compress2(comp, &compLen, orig, ORIG_LEN, 6);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp + compLen;
    dstrm.avail_out = (uInt)(bound - compLen);
    deflate(&dstrm, 0);

    // step 3: Operate & validate (initialize inflate, check sync point, combine CRCs, write via gzdopen)
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    int syncpoint = inflateSyncPoint(&istrm);
    uLong crc_combined = crc32_combine64(crc_orig, crc_orig, (off64_t)ORIG_LEN);
    gzFile gzf = gzdopen(1, "wb");
    gzwrite(gzf, orig, (unsigned int)ORIG_LEN);
    gzclose(gzf);

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    (void)ver;
    (void)compLen;
    (void)bound;
    (void)syncpoint;
    (void)crc_orig;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}