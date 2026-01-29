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
//<ID> 1593
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations (Initialize)
    const char payload[] = "zlib_demo";
    const size_t payloadLen = (size_t)(sizeof(payload) - 1);
    uLong srcLen = (uLong)payloadLen;
    Bytef *src = new Bytef[payloadLen];
    memset(src, 0, payloadLen);
    memcpy(src, payload, payloadLen);
    const char *version = zlibVersion();
    uLong adler = adler32_z(1UL, src, (z_size_t)srcLen);
    uLong crc = crc32_z(0UL, src, (z_size_t)srcLen);
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)srcLen;
    int rc_deflate_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int rc_deflate_prime = deflatePrime(&def_strm, 3, 5);

    // step 2: Configure (open gz file and write bytes)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_putc0 = gzputc(gzf, (int)src[0]);
    int rc_putc1 = gzputc(gzf, (int)src[1]);
    int rc_putc2 = gzputc(gzf, (int)src[2]);

    // step 3: Operate (write checksum-derived bytes and close gz)
    int rc_putc_adler = gzputc(gzf, (int)(adler & 0xFF));
    int rc_putc_crc = gzputc(gzf, (int)(crc & 0xFF));
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and Cleanup
    int rc_deflate_end = deflateEnd(&def_strm);
    delete [] src;
    (void)payloadLen;
    (void)srcLen;
    (void)version;
    (void)adler;
    (void)crc;
    (void)rc_deflate_init;
    (void)rc_deflate_prime;
    (void)rc_putc0;
    (void)rc_putc1;
    (void)rc_putc2;
    (void)rc_putc_adler;
    (void)rc_putc_crc;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}