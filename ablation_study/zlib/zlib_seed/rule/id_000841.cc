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
//<ID> 841
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong TOTAL = 300UL;
    Bytef *buf = (Bytef *)malloc((size_t)TOTAL);
    memset(buf, 'A', (size_t)TOTAL);
    uLong ad1 = adler32(0UL, buf, (uInt)150);
    uLong ad2 = adler32(0UL, buf + 150, (uInt)(TOTAL - 150));
    off_t tail_len = (off_t)(TOTAL - 150);
    uLong ad_combined = adler32_combine(ad1, ad2, tail_len);

    // step 2: Initialize inflate stream and provide input
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int windowBits = 15;
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = buf;
    istrm.avail_in = (uInt)TOTAL;

    // step 3: Retrieve dictionary buffer and adjust undermine setting
    Bytef *dictbuf = (Bytef *)malloc(64);
    memset(dictbuf, 0, 64);
    uInt dictlen = 64U;
    int getdict_ret = inflateGetDictionary(&istrm, dictbuf, &dictlen);
    int undermine_ret = inflateUndermine(&istrm, 1);

    // step 4: Cleanup
    int end_ret = inflateEnd(&istrm);
    free(buf);
    free(dictbuf);
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)getdict_ret;
    (void)undermine_ret;
    (void)end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}