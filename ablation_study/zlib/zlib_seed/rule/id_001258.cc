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
//<ID> 1258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and declare streams
    const uLong ORIG_LEN = 512UL;
    Bytef *src = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(src, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;

    // step 2: Initialize deflate with custom parameters and perform compression
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream)); // method=8 (Z_DEFLATED), strategy=0
    uLong comp_bound = deflateBound(&dstrm, ORIG_LEN);
    uInt comp_buf_len = (uInt)comp_bound;
    Bytef *comp = (Bytef *)malloc((size_t)comp_buf_len);
    memset(comp, 0, (size_t)comp_buf_len);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = comp_buf_len;
    deflate(&dstrm, 4); // flush=4 (Z_FINISH)
    uLong comp_len = dstrm.total_out;

    // step 3: Reset deflate keeping internal state, initialize inflate and perform decompression and validation
    deflateResetKeep(&dstrm);
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0); // flush=0 (Z_NO_FLUSH)
    inflateValidate(&istrm, 1);

    // step 4: Cleanup streams and buffers
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}