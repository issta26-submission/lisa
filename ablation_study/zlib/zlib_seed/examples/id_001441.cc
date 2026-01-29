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
//<ID> 1441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Example payload for zlib crc and gzputc usage";
    const uLong totalLen = (uLong)(sizeof(payload) - 1);
    const uLong len1 = (uLong)(totalLen / 2);
    Bytef *buf = new Bytef[(size_t)totalLen];
    memset(buf, 0, (size_t)totalLen);
    memcpy(buf, payload, (size_t)totalLen);
    const char *version = zlibVersion();
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");

    // step 2: Setup - compute CRCs for two contiguous segments
    uLong crc_part1 = crc32_z(0UL, buf, (z_size_t)len1);
    uLong crc_part2 = crc32_z(0UL, buf + len1, (z_size_t)(totalLen - len1));

    // step 3: Core operations - combine CRCs and emit bytes via gzputc
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)(totalLen - len1));
    int rc_put1 = gzputc(gzf, (int)buf[0]);
    int rc_put2 = gzputc(gzf, (int)(crc_combined & 0xFF));
    int rc_put3 = gzputc(gzf, (int)((crc_combined >> 8) & 0xFF));
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    delete [] buf;
    (void)version;
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    (void)rc_put1;
    (void)rc_put2;
    (void)rc_put3;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}