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
//<ID> 1448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload: crc/combine/gzputc/version";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong crc_src = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 2: Setup - prepare a modified second block to simulate concatenated data
    Bytef *second = new Bytef[(size_t)srcLen];
    memset(second, 0, (size_t)srcLen);
    memcpy(second, src, (size_t)srcLen);
    second[0] = (Byte)('M'); /* small, explicit modification */
    second[1] = (Byte)('-');
    uLong crc_second = crc32_z(0UL, second, (z_size_t)srcLen);

    // step 3: Core operations - combine CRCs and write some bytes to a gzip file using gzputc
    uLong combined_crc = crc32_combine(crc_src, crc_second, (off_t)srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_put0 = gzputc(gzf, (int)src[0]);
    int rc_put1 = gzputc(gzf, (int)src[1]);
    int rc_put2 = gzputc(gzf, (int)second[0]);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    delete [] second;
    (void)version;
    (void)crc_src;
    (void)crc_second;
    (void)combined_crc;
    (void)rc_put0;
    (void)rc_put1;
    (void)rc_put2;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}