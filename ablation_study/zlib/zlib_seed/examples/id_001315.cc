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
//<ID> 1315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib API sequence payload for inflate, adler32 and gz operations";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    const char *version = zlibVersion();
    uLong ad_total = adler32(0UL, src, (uInt)srcLen);

    // step 2: Setup (initialize inflate stream and adjust its window)
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = src;
    istrm.avail_in = (uInt)srcLen;
    int rc_inflate_reset2 = inflateReset2(&istrm, 15);

    // step 3: Core operations (write data to gzip file and seek, validate adler32 incrementally)
    gzFile gzf = gzopen64("zlib_api_seq_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)src, (unsigned int)srcLen);
    off_t off = gzseek(gzf, (off_t)0, SEEK_SET);
    uInt half = (uInt)(srcLen / 2);
    uLong ad_part1 = adler32(0UL, src, half);
    uLong ad_comb = adler32(ad_part1, src + half, (uInt)(srcLen - half));

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    (void)version;
    (void)ad_total;
    (void)rc_inflate_init;
    (void)rc_inflate_reset2;
    (void)rc_gzwrite;
    (void)off;
    (void)ad_part1;
    (void)ad_comb;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}