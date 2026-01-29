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
//<ID> 1259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input data and allocate buffers
    const uLong SRC_LEN = 128UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    uLong decomp_len = SRC_LEN + 16;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);

    // step 2: Initialize and run deflate (compress) with deflateInit2_ and then deflateResetKeep
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size = dstrm.total_out;
    deflateResetKeep(&dstrm);

    // step 3: Initialize inflate, perform inflate, and validate with inflateValidate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateValidate(&istrm, 0);

    // step 4: Cleanup streams and buffers
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}