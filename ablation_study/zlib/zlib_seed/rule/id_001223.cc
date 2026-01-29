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
//<ID> 1223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compute CRCs, then combine
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'B', (size_t)ORIG_LEN);
    uLong part1_len = ORIG_LEN / 2;
    uLong part2_len = ORIG_LEN - part1_len;
    uLong crc1 = crc32(0UL, orig, (uInt)part1_len);
    uLong crc2 = crc32(0UL, orig + part1_len, (uInt)part2_len);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)part2_len);

    // step 2: Initialize and configure deflate and inflate streams
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    // step 3: Operate: deflate data, then initialize inflate and call undermine + back end
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateUndermine(&istrm, 1);
    inflateBackEnd(&istrm);

    // step 4: Cleanup and validate flow (use combined_crc to avoid unused warnings)
    (void)combined_crc;
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    // API sequence test completed successfully
    return 66;
}