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
//<ID> 1318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib api sequence payload for inflate/gz and checksum";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Initialize and configure inflate stream and checksums
    uLong ad_full = adler32(0UL, src, (uInt)srcLen);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    Bytef *outBuf = new Bytef[(size_t)srcLen];
    memset(outBuf, 0, (size_t)srcLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)srcLen;
    istrm.adler = ad_full;
    int rc_reset2 = inflateReset2(&istrm, 15); // set windowBits to 15

    // step 3: Operate (write data to a gz file, seek, and recompute partial checksums)
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)src, (unsigned int)srcLen);
    off_t pos_after_write = gzseek(gzf, 0, 0);
    uInt half = (uInt)(srcLen / 2);
    uLong ad_part1 = adler32(0UL, src, half);
    uLong ad_part2 = adler32(0UL, src + half, (uInt)(srcLen - half));

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] outBuf;
    (void)version;
    (void)rc_inflate_init;
    (void)rc_reset2;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)ad_full;
    (void)ad_part1;
    (void)ad_part2;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}