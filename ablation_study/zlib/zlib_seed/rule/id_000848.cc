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
//<ID> 848
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong TOTAL = 256UL;
    Bytef *buf = (Bytef *)malloc((size_t)TOTAL);
    memset(buf, 'A', (size_t)TOTAL);
    uLong ad1 = adler32(0UL, buf, (uInt)128);
    uLong ad2 = adler32(0UL, buf + 128, (uInt)(TOTAL - 128));
    off_t part2_len = (off_t)(TOTAL - 128);
    uLong ad_combined = adler32_combine(ad1, ad2, part2_len);

    // step 2: Initialize inflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    int windowBits = 15 + 32;
    inflateInit2_(&strm, windowBits, ver, (int)sizeof(z_stream));

    // step 3: Configure stream & query dictionary / undermine safety
    strm.next_in = buf;
    strm.avail_in = (uInt)TOTAL;
    Bytef *dict = (Bytef *)malloc(64);
    memset(dict, 0, 64);
    uInt dictLen = 64;
    int ret_getdict = inflateGetDictionary(&strm, dict, &dictLen);
    int ret_undermine = inflateUndermine(&strm, 1);

    // step 4: Cleanup
    inflateEnd(&strm);
    free(buf);
    free(dict);
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)ret_getdict;
    (void)ret_undermine;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}