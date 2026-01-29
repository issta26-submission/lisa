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
//<ID> 847
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (Initialize)
    const char *ver = zlibVersion();
    const uLong LEN1 = 128UL;
    const uLong LEN2 = 64UL;
    Bytef *in1 = (Bytef *)malloc((size_t)LEN1);
    Bytef *in2 = (Bytef *)malloc((size_t)LEN2);
    memset(in1, 'A', (size_t)LEN1);
    memset(in2, 'B', (size_t)LEN2);
    uLong ad1 = adler32(0UL, in1, (uInt)LEN1);
    uLong ad2 = adler32(0UL, in2, (uInt)LEN2);
    uLong ad_combined = adler32_combine(ad1, ad2, (off_t)LEN2);

    // step 2: Setup inflate stream and buffers (Configure)
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15 + 32, ver, (int)sizeof(z_stream)); /* windowBits: 15 + 32 to enable gzip decoding */
    istrm.next_in = in1;
    istrm.avail_in = (uInt)LEN1;
    const uLong OUTLEN = 256UL;
    Bytef *out = (Bytef *)malloc((size_t)OUTLEN);
    memset(out, 0, (size_t)OUTLEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)OUTLEN;

    // step 3: Core operations (Operate & Validate)
    int inf_status = inflate(&istrm, 0);
    int undermine_status = inflateUndermine(&istrm, 1);
    uInt dictLen = 32U;
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memset(dict, 0, (size_t)dictLen);
    int getdict_status = inflateGetDictionary(&istrm, dict, &dictLen);

    // step 4: Cleanup
    inflateEnd(&istrm);
    free(in1);
    free(in2);
    free(out);
    free(dict);
    (void)ver;
    (void)ad1;
    (void)ad2;
    (void)ad_combined;
    (void)inf_status;
    (void)undermine_status;
    (void)getdict_status;
    // API sequence test completed successfully
    return 66;
}