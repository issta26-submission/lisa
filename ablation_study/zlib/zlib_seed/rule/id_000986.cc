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
//<ID> 986
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 3UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'C', (size_t)ORIG_LEN);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Write bytes to a gzip file using gzputc and close writer with gzclose_w
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzputc(gw, orig[0]);
    gzputc(gw, orig[1]);
    gzputc(gw, orig[2]);
    gzclose_w(gw);

    // step 3: Read back first two bytes using gzgetc_ and compute CRC of read data
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int r0 = gzgetc_(gr);
    int r1 = gzgetc_(gr);
    gzclose(gr);
    Bytef readbuf[3];
    readbuf[0] = (Bytef)r0;
    readbuf[1] = (Bytef)r1;
    readbuf[2] = orig[2];
    uLong crc_read = crc32(0L, readbuf, (uInt)ORIG_LEN);

    // step 4: Initialize an inflate stream, perform reset operations, cleanup everything
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset(&istrm);
    inflateResetKeep(&istrm);
    inflateEnd(&istrm);

    free(orig);
    (void)ver;
    (void)crc_orig;
    (void)crc_read;
    (void)r0;
    (void)r1;
    // API sequence test completed successfully
    return 66;
}