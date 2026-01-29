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
//<ID> 961
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Initialize inflate source stream and copy it
    z_stream src;
    memset(&src, 0, sizeof(src));
    src.zalloc = NULL;
    src.zfree = NULL;
    src.opaque = NULL;
    inflateInit_(&src, ver, (int)sizeof(z_stream));
    src.next_in = NULL;
    src.avail_in = 0;
    src.next_out = decomp;
    src.avail_out = (uInt)ORIG_LEN;
    z_stream dst;
    memset(&dst, 0, sizeof(dst));
    dst.zalloc = NULL;
    dst.zfree = NULL;
    dst.opaque = NULL;
    int copy_ret = inflateCopy(&dst, &src);

    // step 3: Reset copied stream, compute combined CRC and adler on decomp
    inflateReset(&dst);
    uLong crc_combined = crc32_combine64(crc_orig, crc_orig, (off64_t)ORIG_LEN);
    uLong adler_combined = adler32(adler_orig, decomp, (uInt)ORIG_LEN);

    // step 4: Cleanup and end streams
    inflateEnd(&dst);
    inflateEnd(&src);
    free(orig);
    free(decomp);
    (void)ver;
    (void)copy_ret;
    (void)crc_combined;
    (void)adler_combined;
    // API sequence test completed successfully
    return 66;
}