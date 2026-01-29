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
//<ID> 1224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "First payload for zlib API sequence";
    const char payloadB[] = "Second payload for zlib API sequence";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    uLong crcA = crc32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlA = adler32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlB = adler32_z(0UL, (const Bytef *)payloadB, (z_size_t)lenB);
    uLong combined_adl = adler32_combine(adlA, adlB, (off_t)lenB);
    unsigned int bufsize = (unsigned int)((combined_adl & 0xFFFFUL) + 64UL);

    // step 2: Setup (open and configure gz file buffering)
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb+");
    int rc_gzbuffer = gzbuffer(gzf, bufsize);

    // step 3: Core operations (write payloads, push back a byte using crc, clear errors)
    int rc_gzwrite1 = gzwrite(gzf, (const void *)payloadA, (unsigned int)lenA);
    int rc_gzwrite2 = gzwrite(gzf, (const void *)payloadB, (unsigned int)lenB);
    int push_byte = (int)(crcA & 0xFFUL);
    int rc_gzungetc = gzungetc(push_byte, gzf);
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    (void)crcA;
    (void)adlA;
    (void)adlB;
    (void)combined_adl;
    (void)bufsize;
    (void)rc_gzbuffer;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzungetc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}