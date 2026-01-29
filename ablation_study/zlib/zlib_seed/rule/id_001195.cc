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
//<ID> 1195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source buffer and compute adler32 checksum, determine compressed bound
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong ad = adler32_z(0UL, orig, (z_size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size_f = (uLongf)bound;

    // step 2: Initialize deflate and inflate streams and create copies
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

    // step 3: Compress original data, prime the inflate copy, then uncompress using uncompress()
    compress2(comp, &comp_size_f, orig, ORIG_LEN, 6);
    uLong comp_size = (uLong)comp_size_f;
    icopy.next_in = comp;
    icopy.avail_in = (uInt)comp_size;
    inflatePrime(&icopy, 3, 5);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, comp_size);

    // step 4: Cleanup streams and buffers
    deflateEnd(&dcopy);
    deflateEnd(&dstrm);
    inflateEnd(&icopy);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}