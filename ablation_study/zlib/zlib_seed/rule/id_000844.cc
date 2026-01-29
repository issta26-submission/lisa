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
//<ID> 844
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
    const uLong HALF = TOTAL / 2;
    Bytef *buf = (Bytef *)malloc((size_t)TOTAL);
    memset(buf, 'A', (size_t)TOTAL);
    uLong ad1 = adler32(0UL, buf, (uInt)HALF);
    uLong ad2 = adler32(0UL, buf + HALF, (uInt)(TOTAL - HALF));
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)(TOTAL - HALF));

    // step 2: Initialize & configure inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int windowBits = 15;
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)TOTAL;
    istrm.adler = combined_ad;

    // step 3: Operate (undermine and retrieve dictionary)
    int undermine_rc = inflateUndermine(&istrm, 1);
    uInt dictCap = 64U;
    Bytef *dictBuf = (Bytef *)malloc((size_t)dictCap);
    memset(dictBuf, 0, (size_t)dictCap);
    uInt dictLen = dictCap;
    int getdict_rc = inflateGetDictionary(&istrm, dictBuf, &dictLen);

    // step 4: Validate & Cleanup
    (void)undermine_rc;
    (void)getdict_rc;
    (void)dictLen;
    (void)combined_ad;
    (void)ver;
    inflateEnd(&istrm);
    free(dictBuf);
    free(buf);
    // API sequence test completed successfully
    return 66;
}