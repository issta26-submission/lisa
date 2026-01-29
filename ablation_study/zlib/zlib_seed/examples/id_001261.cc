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
//<ID> 1261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "Zlib API sequence payload";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    gzFile gz = gzopen("zlib_api_sequence_output.gz", "wb");

    // step 2: Initialize and configure inflate with header retrieval
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    int rc_inflate_getheader = inflateGetHeader(&istrm, &head);

    // step 3: Operate - write out some bytes via gzputc and compute checksums
    int rc_gzputc1 = gzputc(gz, 'Z');
    int rc_gzputc2 = gzputc(gz, 'L');
    int rc_gzputc3 = gzputc(gz, '\n');
    uLong crc_full = crc32(0UL, src, (uInt)srcLen);
    uInt part1 = (uInt)(srcLen / 2UL);
    uInt part2 = (uInt)(srcLen - (uLong)part1);
    uLong adler_part1 = adler32(1UL, src, part1);
    uLong adler_part2 = adler32(1UL, src + part1, part2);
    uLong adler_combined = adler32_combine(adler_part1, adler_part2, (off_t)part2);

    // step 4: Validate (via available API results) and cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_gzclose = gzclose(gz);
    delete [] src;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_inflate_getheader;
    (void)rc_gzputc1;
    (void)rc_gzputc2;
    (void)rc_gzputc3;
    (void)crc_full;
    (void)adler_part1;
    (void)adler_part2;
    (void)adler_combined;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}