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
//<ID> 863
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
    Bytef *buf = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(buf, 'X', (size_t)ORIG_LEN);
    uInt half = (uInt)(ORIG_LEN / 2UL);
    uLong crc_start = crc32(0UL, buf, half);
    uLong crc_full_via_update = crc32(crc_start, buf + half, (uInt)(ORIG_LEN - half));
    uLong crc_direct = crc32(0UL, buf, (uInt)ORIG_LEN);

    // step 2: Initialize streams
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)ORIG_LEN;

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);

    // step 3: Operate & validate
    gzFile gf = gzdopen(0, "rb");
    (void)gf; // gf may be unused beyond creation in this straight-line test
    uLong crc_after_gzdopen = crc32(crc_full_via_update, buf, (uInt)ORIG_LEN);
    (void)crc_after_gzdopen;
    (void)crc_direct;
    (void)crc_start;
    (void)half;

    // step 4: Cleanup
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    gzclose(gf);
    free(buf);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}