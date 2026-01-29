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
//<ID> 1440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char part1[] = "Hello";
    const char part2[] = " zlib";
    const uLong len1 = (uLong)(sizeof(part1) - 1);
    const uLong len2 = (uLong)(sizeof(part2) - 1);
    Bytef *buf1 = new Bytef[(size_t)len1];
    memset(buf1, 0, (size_t)len1);
    memcpy(buf1, part1, (size_t)len1);
    Bytef *buf2 = new Bytef[(size_t)len2];
    memset(buf2, 0, (size_t)len2);
    memcpy(buf2, part2, (size_t)len2);
    const char *version = zlibVersion();

    // step 2: Setup
    uLong crc_part1 = crc32_z(0UL, buf1, (z_size_t)len1);
    uLong crc_part2 = crc32_z(0UL, buf2, (z_size_t)len2);

    // step 3: Core operations
    uLong crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)len2);
    gzFile gz = gzopen("test_zlib_api_sequence_output.gz", "wb");
    int rc_putc_1 = gzputc(gz, (int)buf1[0]);
    int rc_putc_2 = gzputc(gz, (int)buf2[0]);
    int rc_putc_nl = gzputc(gz, (int)'\n');
    int rc_close = gzclose(gz);

    // step 4: Cleanup
    delete [] buf1;
    delete [] buf2;
    (void)version;
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    (void)rc_putc_1;
    (void)rc_putc_2;
    (void)rc_putc_nl;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}