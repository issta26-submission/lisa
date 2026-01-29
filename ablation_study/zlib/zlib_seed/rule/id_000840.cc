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
//<ID> 840
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
    uInt half = (uInt)(TOTAL / 2UL);
    uLong ad1 = adler32(0UL, inbuf, half);
    uLong ad2 = adler32(0UL, inbuf + half, (uInt)(TOTAL - half));
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)(TOTAL - half));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;

    // step 2: Initialize and configure inflate
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = inbuf;
    istrm.avail_in = (uInt)TOTAL;

    // step 3: Operate & validate: undermine, retrieve dictionary, run a single inflate call
    int undermine_status = inflateUndermine(&istrm, 1);
    uInt dict_buf_size = 64U;
    Bytef *dictbuf = (Bytef *)malloc((size_t)dict_buf_size);
    memset(dictbuf, 0, (size_t)dict_buf_size);
    uInt dict_len = dict_buf_size;
    int getdict_status = inflateGetDictionary(&istrm, dictbuf, &dict_len);
    int inflate_status = inflate(&istrm, 0);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(inbuf);
    free(dictbuf);
    (void)ver;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)undermine_status;
    (void)getdict_status;
    (void)inflate_status;
    // API sequence test completed successfully
    return 66;
}