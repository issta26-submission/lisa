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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and original data
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(comp, 0, (size_t)comp_bound);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize and run deflate (compress) using deflateInit2_ and deflate
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4); // Z_FINISH == 4
    uLong compressed_len = dstrm.total_out;

    // step 3: Initialize inflate stream, validate and inflate (decompress)
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compressed_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateValidate(&istrm, 1);
    inflate(&istrm, 4); // Z_FINISH == 4

    // step 4: Reset deflate keep, cleanup streams and free memory
    deflateResetKeep(&dstrm);
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}