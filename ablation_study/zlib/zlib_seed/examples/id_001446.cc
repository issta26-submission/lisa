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
//<ID> 1446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for crc/combine/gzputc/version";
    const uLong totalLen = (uLong)(sizeof(payload) - 1);
    const size_t bufSize = (size_t)totalLen;
    Bytef *buf = new Bytef[bufSize];
    memset(buf, 0, bufSize);
    memcpy(buf, payload, bufSize);
    const size_t len1_s = (size_t)(totalLen / 2);
    const size_t len2_s = bufSize - len1_s;
    Bytef *part1 = buf;
    Bytef *part2 = buf + len1_s;
    const char *version = zlibVersion();

    // step 2: Setup
    uLong crc_part1 = crc32_z(0UL, part1, (z_size_t)len1_s);
    uLong crc_part2 = crc32_z(0UL, part2, (z_size_t)len2_s);
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)len2_s);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");

    // step 3: Core operations
    int rc_put0 = gzputc(gzf, (int)part1[0]);
    int rc_put1 = gzputc(gzf, (int)part2[0]);

    // step 4: Cleanup
    int rc_close = gzclose(gzf);
    delete [] buf;
    (void)version;
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    (void)rc_put0;
    (void)rc_put1;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}