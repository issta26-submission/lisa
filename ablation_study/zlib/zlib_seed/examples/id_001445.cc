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
//<ID> 1445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "ABCD";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup & Configure
    uLong crc_first = crc32_z(0UL, src, (z_size_t)srcLen);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");

    // step 3: Operate & Validate
    int rc_put0 = gzputc(gzf, (int)src[0]);
    int rc_put1 = gzputc(gzf, (int)src[1]);
    int rc_put2 = gzputc(gzf, (int)src[2]);
    int rc_put3 = gzputc(gzf, (int)src[3]);
    int rc_put4 = gzputc(gzf, (int)src[0]);
    int rc_put5 = gzputc(gzf, (int)src[1]);
    int rc_put6 = gzputc(gzf, (int)src[2]);
    int rc_put7 = gzputc(gzf, (int)src[3]);
    uLong crc_second = crc32_z(0UL, src, (z_size_t)srcLen);
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)srcLen);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    (void)version;
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)rc_put0; (void)rc_put1; (void)rc_put2; (void)rc_put3;
    (void)rc_put4; (void)rc_put5; (void)rc_put6; (void)rc_put7;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}