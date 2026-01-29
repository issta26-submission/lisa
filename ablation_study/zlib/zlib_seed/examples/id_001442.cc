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
//<ID> 1442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api usage payload for crc and gzputc";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();

    // step 2: Setup - compute checksums and partition lengths
    uLong crc_total = crc32_z(0UL, src, (z_size_t)srcLen);
    const uLong firstLen = 1UL;
    const off_t restLen = (off_t)(srcLen - firstLen);
    uLong crc_first = crc32_z(0UL, src, (z_size_t)firstLen);
    uLong crc_rest = crc32_z(0UL, src + firstLen, (z_size_t)(srcLen - firstLen));
    uLong crc_combined = crc32_combine(crc_first, crc_rest, restLen);

    // step 3: Core operations - write some bytes to a gzip file using gzputc
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_put0 = gzputc(gzf, (int)src[0]);
    int rc_put1 = gzputc(gzf, (int)src[1]);
    int rc_put_last = gzputc(gzf, (int)src[srcLen - 1]);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    delete [] src;
    (void)version;
    (void)crc_total;
    (void)crc_first;
    (void)crc_rest;
    (void)crc_combined;
    (void)rc_put0;
    (void)rc_put1;
    (void)rc_put_last;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}