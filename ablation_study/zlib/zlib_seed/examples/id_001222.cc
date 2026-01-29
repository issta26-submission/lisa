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
//<ID> 1222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Hello_zlib_A";
    const char payloadB[] = "World_zlib_B";
    const z_size_t lenA = (z_size_t)(sizeof(payloadA) - 1);
    const z_size_t lenB = (z_size_t)(sizeof(payloadB) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    gzFile gzf = gzopen("zlib_api_seq_test.gz", "wb+");

    // step 2: Setup (initialize deflate and compute checksums)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong crcA = crc32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlA = adler32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlB = adler32_z(0UL, (const Bytef *)payloadB, (z_size_t)lenB);

    // step 3: Core operations (write gz content, combine checksums, push back a byte and clear errors)
    int rc_gzwrite1 = gzwrite(gzf, (voidpc)payloadA, (unsigned int)(lenA ? lenA : 0));
    int rc_gzwrite2 = gzwrite(gzf, (voidpc)payloadB, (unsigned int)(lenB ? lenB : 0));
    uLong combined_adler = adler32_combine(adlA, adlB, (off_t)lenB);
    int rc_ungetc = gzungetc((int)(unsigned char)payloadB[0], gzf);
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    (void)version;
    (void)crcA;
    (void)combined_adler;
    (void)rc_deflate_init;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_ungetc;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}