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
//<ID> 845
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong ad1 = adler32(0UL, input, (uInt)(INPUT_LEN / 2));
    uLong ad2 = adler32(0UL, input + (INPUT_LEN / 2), (uInt)(INPUT_LEN - (INPUT_LEN / 2)));
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)(INPUT_LEN - (INPUT_LEN / 2)));

    // step 2: Initialize deflate and compress input
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong compSize = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate with custom windowBits and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compSize;
    Bytef *out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(out, 0, (size_t)INPUT_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)INPUT_LEN;
    inflate(&istrm, 4); /* Z_FINISH */

    // step 4: Validate dictionary-related APIs and cleanup
    uInt dictLen = (uInt)INPUT_LEN;
    Bytef *dictBuf = (Bytef *)malloc((size_t)dictLen);
    memset(dictBuf, 0, (size_t)dictLen);
    inflateGetDictionary(&istrm, dictBuf, &dictLen);
    inflateUndermine(&istrm, 0);
    inflateEnd(&istrm);
    free(input);
    free(comp);
    free(out);
    free(dictBuf);
    (void)ver;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)bound;
    (void)compSize;
    (void)dictLen;
    // API sequence test completed successfully
    return 66;
}