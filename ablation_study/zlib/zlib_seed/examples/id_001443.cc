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
//<ID> 1443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload1[] = "partA_for_crc_and_gzputc";
    const char payload2[] = "partB_for_crc_combine";
    const uLong len1 = (uLong)(sizeof(payload1) - 1);
    const uLong len2 = (uLong)(sizeof(payload2) - 1);
    Bytef *part1 = new Bytef[(size_t)len1];
    Bytef *part2 = new Bytef[(size_t)len2];
    memset(part1, 0, (size_t)len1);
    memset(part2, 0, (size_t)len2);
    memcpy(part1, payload1, (size_t)len1);
    memcpy(part2, payload2, (size_t)len2);
    const char *version = zlibVersion();

    // step 2: Setup - compute individual CRCs
    uLong crc1 = crc32_z(0UL, part1, (z_size_t)len1);
    uLong crc2 = crc32_z(0UL, part2, (z_size_t)len2);

    // step 3: Core operations - combine CRCs and write a byte to a gzip file
    uLong crc_combined = crc32_combine(crc1, crc2, (off_t)len2);
    gzFile gzf = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_put = gzputc(gzf, (int)part1[0]);
    int rc_close = gzclose(gzf);

    // step 4: Cleanup
    delete [] part1;
    delete [] part2;
    (void)version;
    (void)crc1;
    (void)crc2;
    (void)crc_combined;
    (void)rc_put;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}