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
//<ID> 862
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong BUFLEN = 512UL;
    Bytef *buf = (Bytef *)malloc((size_t)BUFLEN);
    memset(buf, 'X', (size_t)BUFLEN);
    uLong crc_before = crc32(0UL, buf, (uInt)BUFLEN);

    // step 2: Initialize deflate stream and exercise deflateResetKeep
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.adler = crc_before;
    deflateResetKeep(&dstrm);

    // step 3: Initialize inflate stream, associate input, and open gz handle via gzdopen
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)BUFLEN;
    istrm.adler = crc_before;
    gzFile gz = gzdopen(1, "wb");

    // step 4: Validate & Cleanup
    uLong crc_after = crc32(crc_before, buf, (uInt)BUFLEN);
    (void)crc_after;
    if (gz != NULL) { gzclose(gz); } else { /* no-op to keep straight-line semantics */ }
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(buf);
    (void)ver;
    (void)crc_before;
    // API sequence test completed successfully
    return 66;
}