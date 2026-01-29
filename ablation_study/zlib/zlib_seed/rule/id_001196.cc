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
//<ID> 1196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and metadata
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong ad_orig = adler32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Initialize deflate and inflate streams
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));

    // step 3: Compress orig, uncompress into dest, copy streams and prime inflate
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size_f = (uLongf)bound;
    compress(comp, &comp_size_f, orig, ORIG_LEN);
    uLong comp_size = (uLong)comp_size_f;

    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_size);

    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, sizeof(dstrm_copy));
    dstrm_copy.zalloc = NULL;
    dstrm_copy.zfree = NULL;
    dstrm_copy.opaque = NULL;
    deflateCopy(&dstrm_copy, &dstrm);

    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    istrm_copy.zalloc = NULL;
    istrm_copy.zfree = NULL;
    istrm_copy.opaque = NULL;
    inflateCopy(&istrm_copy, &istrm);

    inflatePrime(&istrm, 3, 5);

    uLong ad_decomp = adler32_z(0UL, decomp, (z_size_t)decomp_len);
    (void)ad_orig;
    (void)ad_decomp;

    // step 4: Cleanup streams and buffers
    deflateEnd(&dstrm_copy);
    deflateEnd(&dstrm);
    inflateEnd(&istrm_copy);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}