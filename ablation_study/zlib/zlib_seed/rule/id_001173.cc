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
//<ID> 1173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and compress it with compress()
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress(comp, &comp_len, orig, ORIG_LEN);
    uLong comp_used = (uLong)comp_len;

    // step 2: Initialize a deflate stream (configuration phase)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    // step 3: Initialize an inflate stream, apply undermine and validate, then one-shot decompress with uncompress2
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateUndermine(&istrm, 1);
    inflateValidate(&istrm, 0);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = (uLongf)ORIG_LEN;
    uLong src_len = comp_used;
    uncompress2(decomp, &decomp_len, comp, &src_len);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}