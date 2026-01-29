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
//<ID> 1227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "zlib_seq_partA";
    const char payloadB[] = "zlib_seq_partB";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    unsigned char *in_buf = new unsigned char[256];
    unsigned char *out_buf = new unsigned char[256];
    memset(in_buf, 0, (size_t)256);
    memset(out_buf, 0, (size_t)256);

    // step 2: Setup
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong crcA = crc32_z(0UL, (const Bytef *)payloadA, (z_size_t)lenA);
    uLong adlA = adler32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong adlB = adler32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong combined_adler = adler32_combine(adlA, adlB, (off_t)lenB);
    dstrm.next_in = (Bytef *)in_buf;
    dstrm.avail_in = 0;
    dstrm.next_out = (Bytef *)out_buf;
    dstrm.avail_out = (uInt)256;
    dstrm.adler = combined_adler;
    dstrm.total_in = crcA;

    // step 3: Core operations
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb+");
    int rc_gzwrite1 = gzwrite(gzf, (voidpc)payloadA, (unsigned int)(lenA ? lenA : 0));
    int rc_gzwrite2 = gzwrite(gzf, (voidpc)payloadB, (unsigned int)(lenB ? lenB : 0));
    int rc_unget = gzungetc('X', gzf);
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gzf);
    delete [] in_buf;
    delete [] out_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)crcA;
    (void)adlA;
    (void)adlB;
    (void)combined_adler;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_unget;
    (void)rc_deflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}