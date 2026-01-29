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
//<ID> 1225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "zlib-part-A";
    const char payloadB[] = "++zlib-part-B";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const char *version = zlibVersion();
    unsigned char *read_buf = new unsigned char[256];
    memset(read_buf, 0, (size_t)256);
    uLong crcA = crc32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong crcCombined = crc32_z(crcA, (const Bytef *)payloadB, (z_size_t)lenB);
    uLong adlA = adler32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlB = adler32_z(0UL, (const Bytef *)payloadB, (z_size_t)lenB);
    uLong adlCombined = adler32_combine(adlA, adlB, (off_t)lenB);

    // step 2: Setup (open gz for read/write and write payloads)
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb+");
    int rc_write1 = gzwrite(gzf, (const void *)payloadA, (unsigned int)(lenA ? lenA : 0));
    int rc_write2 = gzwrite(gzf, (const void *)payloadB, (unsigned int)(lenB ? lenB : 0));
    int rc_rewind = gzrewind(gzf);

    // step 3: Core operations (push back a byte, read back data, clear any gz errors)
    int rc_unget = gzungetc((int)payloadA[0], gzf);
    unsigned int want_read = (unsigned int)(lenA + lenB + 8);
    int rc_read = gzread(gzf, (void *)read_buf, want_read);
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    int rc_close = gzclose(gzf);
    delete [] read_buf;
    (void)version;
    (void)crcA;
    (void)crcCombined;
    (void)adlA;
    (void)adlB;
    (void)adlCombined;
    (void)rc_write1;
    (void)rc_write2;
    (void)rc_rewind;
    (void)rc_unget;
    (void)rc_read;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}