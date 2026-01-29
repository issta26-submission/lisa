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
//<ID> 981
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong BUF_LEN = 128UL;
    Bytef *buf = (Bytef *)malloc((size_t)BUF_LEN);
    memset(buf, 'A', (size_t)BUF_LEN);
    Bytef *readBuf = (Bytef *)malloc((size_t)BUF_LEN);
    memset(readBuf, 0, (size_t)BUF_LEN);

    // step 2: Write a couple of bytes into a gzip file using gzputc and close writer
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int put1 = gzputc(gw, 'H');
    int put2 = gzputc(gw, 'i');
    int close_w_res = gzclose_w(gw);

    // step 3: Read back bytes using gzgetc_ and compute crc32 over the read bytes
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int c1 = gzgetc_(gr);
    int c2 = gzgetc_(gr);
    readBuf[0] = (Bytef)(c1 & 0xFF);
    readBuf[1] = (Bytef)(c2 & 0xFF);
    uLong crc_val = crc32(0L, readBuf, (uInt)2);
    int close_r_res = gzclose_r(gr);

    // step 4: Initialize inflate stream, perform resets and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = 0;
    istrm.next_out = readBuf + 2;
    istrm.avail_out = (uInt)(BUF_LEN - 2);
    int r_reset = inflateReset(&istrm);
    int r_reset_keep = inflateResetKeep(&istrm);
    inflateEnd(&istrm);
    free(buf);
    free(readBuf);
    (void)ver;
    (void)put1;
    (void)put2;
    (void)close_w_res;
    (void)c1;
    (void)c2;
    (void)crc_val;
    (void)close_r_res;
    (void)r_reset;
    (void)r_reset_keep;
    // API sequence test completed successfully
    return 66;
}