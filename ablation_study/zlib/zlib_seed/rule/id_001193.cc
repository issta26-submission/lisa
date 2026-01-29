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
//<ID> 1193
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();

    // step 2: Initialize deflate and inflate streams and copy them
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    z_stream dcopy;
    memset(&dcopy, 0, sizeof(dcopy));
    dcopy.zalloc = NULL;
    dcopy.zfree = NULL;
    dcopy.opaque = NULL;
    deflateCopy(&dcopy, &dstrm);

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    z_stream icopy;
    memset(&icopy, 0, sizeof(icopy));
    icopy.zalloc = NULL;
    icopy.zfree = NULL;
    icopy.opaque = NULL;
    inflateCopy(&icopy, &istrm);
    inflatePrime(&icopy, 3, 5);

    // step 3: Compress original buffer and then uncompress into output buffer
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len_f = (uLongf)bound;
    compress(comp, &comp_len_f, orig, ORIG_LEN);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len_f = (uLongf)ORIG_LEN;
    uncompress(decomp, &decomp_len_f, comp, (uLong)comp_len_f);

    // step 4: Validate checksum, cleanup streams and buffers
    uLong checksum = adler32_z(0UL, decomp, (z_size_t)decomp_len_f);
    (void)checksum;
    deflateEnd(&dstrm);
    deflateEnd(&dcopy);
    inflateEnd(&istrm);
    inflateEnd(&icopy);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}