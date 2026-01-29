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
//<ID> 1594
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib sequence payload for deflatePrime, checksums, gzputc and gzclose";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);

    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    Bytef *outBuf = new Bytef[256];
    memset(outBuf, 0, 256);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)256;
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (prime deflate output and compute checksums)
    int rc_prime = deflatePrime(&strm, 3, 5); // insert a few bits into the output bit buffer
    uLong adler = adler32_z(1UL, src, (z_size_t)payloadLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)payloadLen);

    // step 3: Operate (write selected payload bytes and checksum bytes to a gz file)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_putc0 = gzputc(gzf, (int)payload[0]);
    int rc_putc1 = gzputc(gzf, (int)payload[1]);
    int rc_putc_adler = gzputc(gzf, (int)(adler & 0xFF));
    int rc_putc_crc = gzputc(gzf, (int)(crc & 0xFF));
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&strm);
    delete [] src;
    delete [] outBuf;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_prime;
    (void)adler;
    (void)crc;
    (void)rc_putc0;
    (void)rc_putc1;
    (void)rc_putc_adler;
    (void)rc_putc_crc;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}