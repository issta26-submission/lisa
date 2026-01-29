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
//<ID> 1447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload using crc32_z, crc32_combine, zlibVersion and gzputc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup - open gzip file and configure buffer
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_gzbuffer = gzbuffer(gzf, 4096);

    // step 3: Core operations - compute CRCs, write some bytes via gzputc, combine CRCs for validation
    size_t first_len = (size_t)(srcLen / 2);
    z_size_t zfirst_len = (z_size_t)first_len;
    z_size_t zsecond_len = (z_size_t)(srcLen - (uLong)first_len);
    uLong crc_first = crc32_z(0UL, src, zfirst_len);
    uLong crc_second = crc32_z(0UL, src + first_len, zsecond_len);
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)zsecond_len);
    uLong crc_full = crc32_z(0UL, src, (z_size_t)srcLen);
    int rc_putc0 = gzputc(gzf, (int)src[0]);
    int rc_putc1 = gzputc(gzf, (int)src[first_len]);
    int rc_putc2 = gzputc(gzf, '\n');

    // step 4: Cleanup
    int rc_close = gzclose_w(gzf);
    delete [] src;
    (void)version;
    (void)rc_gzbuffer;
    (void)rc_putc0;
    (void)rc_putc1;
    (void)rc_putc2;
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)crc_full;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}