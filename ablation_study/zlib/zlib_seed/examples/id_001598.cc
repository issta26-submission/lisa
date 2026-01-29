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
//<ID> 1598
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib api sequence payload for deflatePrime, adler32_z, crc32_z, gzputc and gzclose";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong adler = adler32_z(0UL, src, (z_size_t)srcLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Setup (deflate stream initialization and buffers)
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    int rc_def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, srcLen);
    size_t outBufSize = (size_t)(bound + 16);
    Bytef *compBuf = new Bytef[outBufSize];
    memset(compBuf, 0, outBufSize);
    def_strm.next_out = compBuf;
    def_strm.avail_out = (uInt)outBufSize;

    // step 3: Core operations (prime deflate, perform a deflate, write checksum bytes to gz)
    int rc_prime = deflatePrime(&def_strm, 3, 5);
    int rc_deflate = deflate(&def_strm, 0);
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_putc1 = gzputc(gzf, (int)payload[0]);
    int rc_putc2 = gzputc(gzf, (int)payload[payloadLen - 1]);
    int rc_putc3 = gzputc(gzf, (int)(adler & 0xFF));
    int rc_putc4 = gzputc(gzf, (int)(crc & 0xFF));

    // step 4: Validate and Cleanup
    int rc_def_end = deflateEnd(&def_strm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)adler;
    (void)crc;
    (void)rc_def_init;
    (void)bound;
    (void)outBufSize;
    (void)rc_prime;
    (void)rc_deflate;
    (void)rc_putc1;
    (void)rc_putc2;
    (void)rc_putc3;
    (void)rc_putc4;
    (void)rc_def_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}