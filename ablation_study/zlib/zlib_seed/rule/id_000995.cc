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
//<ID> 995
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong crc_part1;
    uLong crc_part2;
    uLong crc_combined;

    // step 2: Write original data to a gzip file
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzclose_w(gw);

    // step 3: Compute CRCs and combine them
    crc_part1 = crc32(0L, orig, (uInt)ORIG_LEN);
    crc_part2 = crc32(0L, orig + (ORIG_LEN / 2), (uInt)(ORIG_LEN / 2));
    crc_combined = crc32_combine(crc_part1, crc_part2, (off_t)(ORIG_LEN / 2));

    // step 4: Initialize inflate, reset it, and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset(&istrm);
    inflateEnd(&istrm);

    free(orig);
    (void)ver;
    (void)crc_part1;
    (void)crc_part2;
    (void)crc_combined;
    // API sequence test completed successfully
    return 66;
}