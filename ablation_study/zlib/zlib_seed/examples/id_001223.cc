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
//<ID> 1223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload1[] = "Hello zlib";
    const char payload2[] = "MoreData";
    const uLong len1 = (uLong)(sizeof(payload1) - 1);
    const uLong len2 = (uLong)(sizeof(payload2) - 1);
    const char *version = zlibVersion();
    uLong crc_part2 = 0UL;
    uLong adler_part1 = 0UL;
    uLong adler_part2 = 0UL;
    uLong combined_adler = 0UL;
    gzFile gzf = (gzFile)0;
    int rc_write1 = 0;
    int rc_write2 = 0;
    int rc_ungetc = 0;

    // step 2: Setup (compute checksums and combine)
    crc_part2 = crc32_z(0UL, (const Bytef *)payload2, (z_size_t)len2);
    adler_part1 = adler32(0UL, (const Bytef *)payload1, (uInt)len1);
    adler_part2 = adler32(0UL, (const Bytef *)payload2, (uInt)len2);
    combined_adler = adler32_combine(adler_part1, adler_part2, (off_t)len2);

    // step 3: Core operations (open gz file, write payloads, push back a byte, clear errors)
    gzf = gzopen("zlib_api_sequence_output.gz", "wb+");
    rc_write1 = gzwrite(gzf, (voidpc)payload1, (unsigned int)(len1 ? len1 : 0));
    rc_write2 = gzwrite(gzf, (voidpc)payload2, (unsigned int)(len2 ? len2 : 0));
    rc_ungetc = gzungetc((int)(combined_adler & 0xFF), gzf);
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    int rc_close = gzclose(gzf);
    (void)version;
    (void)crc_part2;
    (void)adler_part1;
    (void)adler_part2;
    (void)combined_adler;
    (void)rc_write1;
    (void)rc_write2;
    (void)rc_ungetc;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}