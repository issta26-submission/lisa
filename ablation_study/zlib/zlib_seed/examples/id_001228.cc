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
//<ID> 1228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "ZLIB_PART_A_SAMPLE";
    const char payloadB[] = "ZLIB_PART_B_SAMPLE";
    z_size_t lenA = (z_size_t)(sizeof(payloadA) - 1);
    z_size_t lenB = (z_size_t)(sizeof(payloadB) - 1);
    const char *version = zlibVersion();
    unsigned char *tmp_buf = new unsigned char[256];
    memset(tmp_buf, 0, (size_t)256);

    // step 2: Setup (compute checksums)
    uLong crcA = crc32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong crcB = crc32_z(0UL, (const Bytef *)payloadB, (z_size_t)lenB);
    uLong adlA = adler32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlB = adler32_z(0UL, (const Bytef *)payloadB, (z_size_t)lenB);
    uLong combined_adler = adler32_z(adlA, (const Bytef *)payloadB, (z_size_t)lenB);

    // step 3: Core operations (write then close, reopen for read, rewind, read, pushback, clear error)
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int w1 = gzwrite(gzf, (void *)payloadA, (unsigned int)(lenA ? lenA : 0));
    int w2 = gzwrite(gzf, (void *)payloadB, (unsigned int)(lenB ? lenB : 0));
    int rc_close1 = gzclose(gzf);
    gzFile gzf2 = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int rc_rewind = gzrewind(gzf2);
    int first_char = gzgetc(gzf2);
    int rc_ungetc = gzungetc(first_char, gzf2);
    int second_char = gzgetc(gzf2);
    gzclearerr(gzf2);

    // step 4: Validate and cleanup
    int rc_close2 = gzclose(gzf2);
    delete [] tmp_buf;
    (void)version;
    (void)crcA;
    (void)crcB;
    (void)adlA;
    (void)adlB;
    (void)combined_adler;
    (void)w1;
    (void)w2;
    (void)rc_close1;
    (void)rc_rewind;
    (void)first_char;
    (void)rc_ungetc;
    (void)second_char;
    (void)rc_close2;
    // API sequence test completed successfully
    return 66;
}