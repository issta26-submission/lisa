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
//<ID> 1172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original data and buffers
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLongf comp_bound = (uLongf)compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_size = comp_bound;

    // step 2: Compress original data with compress() and also initialize a deflate stream to query deflateBound
    compress(comp, &comp_size, orig, ORIG_LEN);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong dbound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp2 = (Bytef *)malloc((size_t)dbound);
    memset(comp2, 0, (size_t)dbound);
    memcpy(comp2, comp, (size_t)((comp_size < (uLongf)dbound) ? comp_size : (uLongf)dbound));
    deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, call undermine & validate, then uncompress with uncompress2
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateUndermine(&istrm, 1);
    inflateValidate(&istrm, 1);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = (uLongf)ORIG_LEN;
    uLong src_len = (uLong)comp_size;
    uncompress2(decomp, &decomp_len, comp, &src_len);
    inflateEnd(&istrm);

    // step 4: Cleanup
    free(orig);
    free(comp);
    free(comp2);
    free(decomp);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}