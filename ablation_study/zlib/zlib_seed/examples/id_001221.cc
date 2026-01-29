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
//<ID> 1221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char partA[] = "PartA_of_payload_for_zlib";
    const char partB[] = "PartB_of_payload_for_zlib";
    const uLong lenA = (uLong)(sizeof(partA) - 1);
    const uLong lenB = (uLong)(sizeof(partB) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *buffer = new unsigned char[1024];
    memset(buffer, 0, (size_t)1024);
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb+");

    // step 2: Setup (initialize streams and compute checksums)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong adlA = adler32(0UL, (const Bytef *)partA, (uInt)lenA);
    uLong adlB = adler32(0UL, (const Bytef *)partB, (uInt)lenB);
    uLong crcA = crc32_z(0UL, (const Bytef *)partA, (z_size_t)lenA);
    dstrm.next_out = buffer;
    dstrm.avail_out = (uInt)1024;
    istrm.next_in = buffer;
    istrm.avail_in = 0;

    // step 3: Core operations (write compressed data, pushback, clear error, combine checksums)
    int rc_write1 = gzwrite(gzf, (const void *)partA, (unsigned int)(lenA ? lenA : 0));
    int rc_write2 = gzwrite(gzf, (const void *)partB, (unsigned int)(lenB ? lenB : 0));
    int rc_unget = gzungetc('X', gzf);
    gzclearerr(gzf);
    off_t combined_len = (off_t)(lenA + lenB);
    uLong combined_adl = adler32_combine(adlA, adlB, combined_len);
    uLong crc_combined = crc32_z(crcA, (const Bytef *)partB, (z_size_t)lenB);
    dstrm.adler = combined_adl;
    istrm.adler = adlB;
    dstrm.total_in = crc_combined;

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] buffer;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)adlA;
    (void)adlB;
    (void)crcA;
    (void)rc_write1;
    (void)rc_write2;
    (void)rc_unget;
    (void)combined_len;
    (void)combined_adl;
    (void)crc_combined;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}