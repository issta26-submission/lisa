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
//<ID> 1192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare original buffer and compress it with compress()
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLongf comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_size_f = comp_bound;
    compress(comp, &comp_size_f, orig, ORIG_LEN);

    // step 2: Initialize deflate and inflate streams and make copies with deflateCopy/inflateCopy
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    z_stream dstrm_copy;
    memset(&dstrm_copy, 0, sizeof(dstrm_copy));
    deflateCopy(&dstrm_copy, &dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    z_stream istrm_copy;
    memset(&istrm_copy, 0, sizeof(istrm_copy));
    inflateCopy(&istrm_copy, &istrm);

    // step 3: Prime the copied inflate stream, compute adler32_z on original data
    inflatePrime(&istrm_copy, 3, 1);
    uLong ad = adler32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 4: Decompress with uncompress, cleanup streams and buffers
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_size_f);
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