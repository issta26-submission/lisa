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
//<ID> 1226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload1[] = "zlib_payload_part_one";
    const char payload2[] = "zlib_payload_part_two";
    const uLong len1 = (uLong)(sizeof(payload1) - 1);
    const z_size_t len2 = (z_size_t)(sizeof(payload2) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    unsigned char *in_buf = new unsigned char[1024];
    unsigned char *out_buf = new unsigned char[1024];
    memset(in_buf, 0, (size_t)1024);
    memset(out_buf, 0, (size_t)1024);

    // step 2: Setup (initialize streams and compute checksums)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uLong crc_part = crc32_z(0UL, (const Bytef *)payload1, (z_size_t)len1);
    uLong adl_part1 = adler32_z(0UL, (const Bytef *)payload1, (z_size_t)len1);
    uLong adl_part2 = adler32_z(0UL, (const Bytef *)payload2, (z_size_t)len2);
    uLong combined_adler = adler32_combine(adl_part1, adl_part2, (off_t)len2);
    dstrm.next_in = (Bytef *)in_buf;
    dstrm.avail_in = 0;
    dstrm.next_out = (Bytef *)out_buf;
    dstrm.avail_out = (uInt)1024;
    istrm.next_in = (Bytef *)in_buf;
    istrm.avail_in = 0;
    istrm.adler = combined_adler;
    dstrm.total_in = crc_part;

    // step 3: Core operations (write gz, push back a byte, clear errors)
    gzFile gzf = gzopen("zlib_api_sequence_test.gz", "wb+");
    int rc_gzwrite1 = gzwrite(gzf, (voidpc)payload1, (unsigned int)(len1 ? len1 : 0));
    int rc_gzwrite2 = gzwrite(gzf, (voidpc)payload2, (unsigned int)(len2 ? (unsigned int)len2 : 0));
    int rc_gzungetc = gzungetc('X', gzf);
    gzclearerr(gzf);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gzf);
    delete [] in_buf;
    delete [] out_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)rc_inflate_init;
    (void)crc_part;
    (void)adl_part1;
    (void)adl_part2;
    (void)combined_adler;
    (void)rc_gzwrite1;
    (void)rc_gzwrite2;
    (void)rc_gzungetc;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}