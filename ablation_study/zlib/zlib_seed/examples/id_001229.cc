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
//<ID> 1229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Alpha_payload_for_zlib";
    const char payloadB[] = "Beta_payload_for_zlib";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *work_buf = new unsigned char[256];
    memset(work_buf, 0, (size_t)256);

    // step 2: Setup (initialize stream and compute checksums)
    int rc_deflate_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong crcA = crc32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlA = adler32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlB = adler32_z(0UL, (const Bytef *)payloadB, (z_size_t)lenB);
    uLong combined_adler = adler32_combine(adlA, adlB, (off_t)lenB);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb+");

    // step 3: Core operations (write data, push back a byte, clear errors)
    int rc_gzwrite1 = gzwrite(gzf, (const void *)payloadA, (unsigned int)(lenA ? lenA : 0));
    int rc_gzwrite2 = gzwrite(gzf, (const void *)payloadB, (unsigned int)(lenB ? lenB : 0));
    int rc_flush = gzflush(gzf, 0);
    int rc_ungetc = gzungetc((int)payloadB[0], gzf);
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    strm.adler = combined_adler;
    strm.total_in = crcA;
    int rc_deflate_end = deflateEnd(&strm);
    int rc_gzclose = gzclose(gzf);
    delete [] work_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)crcA;
    (void)adlA;
    (void)adlB;
    (void)combined_adler;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_flush;
    (void)rc_ungetc;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}