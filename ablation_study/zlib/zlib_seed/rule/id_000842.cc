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
//<ID> 842
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
    Bytef *inbuf = (Bytef *)malloc((size_t)TOTAL);
    memset(inbuf, 'A', (size_t)TOTAL);
    uInt half = (uInt)(TOTAL / 2);
    uLong a1 = adler32(1UL, inbuf, half);
    uLong a2 = adler32(0UL, inbuf + half, (uInt)(TOTAL - half));
    uLong combined = adler32_combine(a1, a2, (off_t)(TOTAL - half));

    // step 2: Initialize inflate stream
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = inbuf;
    istrm.avail_in = (uInt)TOTAL;

    // step 3: Configure/Operate/Validate
    inflateUndermine(&istrm, 1);
    const uInt DICT_SZ = 128U;
    Bytef *dictBuf = (Bytef *)malloc((size_t)DICT_SZ);
    memset(dictBuf, 0, (size_t)DICT_SZ);
    uInt dictLen = DICT_SZ;
    int rc = inflateGetDictionary(&istrm, dictBuf, &dictLen);
    uLong validation = combined ^ (uLong)dictLen;

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(inbuf);
    free(dictBuf);
    (void)ver;
    (void)a1;
    (void)a2;
    (void)rc;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}