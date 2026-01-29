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
//<ID> 983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'B', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();

    // step 2: Write a few bytes to a gzip file using gzputc and close writer
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gw, orig[0]);
    gzputc(gw, orig[1]);
    gzputc(gw, orig[2]);
    gzputc(gw, orig[3]);
    gzclose_w(gw);

    // step 3: Initialize inflate stream and exercise inflateReset and inflateResetKeep
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = NULL;
    istrm.avail_in = 0;
    inflateReset(&istrm);
    inflateResetKeep(&istrm);
    inflateEnd(&istrm);

    // step 4: Read a byte back with gzgetc_, compute crc32s, and cleanup
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int c = gzgetc_(gr);
    gzclose(gr);
    uLong crc_full = crc32(0L, orig, (uInt)ORIG_LEN);
    Bytef one_byte = (Bytef)c;
    uLong crc_one = crc32(0L, &one_byte, (uInt)1);
    free(orig);
    (void)ver;
    (void)c;
    (void)crc_full;
    (void)crc_one;
    // API sequence test completed successfully
    return 66;
}