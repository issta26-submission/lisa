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
//<ID> 849
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
    memset(buf, 'Z', (size_t)TOTAL);
    const uInt HALF = (uInt)(TOTAL / 2U);
    uLong adl_first = adler32(1UL, buf, HALF);
    uLong adl_second = adler32(0UL, buf + HALF, (uInt)(TOTAL - HALF));
    uLong adl_combined = adler32_combine(adl_first, adl_second, (off_t)(TOTAL - HALF));

    // step 2: Initialize inflate stream and configure
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int windowBits = 15; /* typical default windowBits */
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)TOTAL;
    istrm.adler = adl_combined;

    // step 3: Operate & validate
    uInt dictCap = 128U;
    Bytef *dictBuf = (Bytef *)malloc((size_t)dictCap);
    memset(dictBuf, 0, (size_t)dictCap);
    inflateUndermine(&istrm, 1);
    inflateGetDictionary(&istrm, dictBuf, &dictCap);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(buf);
    free(dictBuf);
    (void)ver;
    (void)adl_first;
    (void)adl_second;
    (void)adl_combined;
    (void)dictCap;
    // API sequence test completed successfully
    return 66;
}