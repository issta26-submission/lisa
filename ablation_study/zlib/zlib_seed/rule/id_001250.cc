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
//<ID> 1250
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and buffers
    const uLong SRC_LEN = 64UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;

    // step 2: Initialize and run deflate (compress)
    deflateInit2_(&dstrm, 6, 8 /* Z_DEFLATED */, 15, 8, 0, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    uLong bound = deflateBound(&dstrm, SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)(bound + 16));
    memset(comp, 0, (size_t)(bound + 16));
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)(bound + 16);
    deflate(&dstrm, 4 /* Z_FINISH */);
    uLong comp_len = dstrm.total_out;
    deflateResetKeep(&dstrm);

    // step 3: Initialize inflate, validate, and inflate (decompress)
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateValidate(&istrm, 1);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *out = (Bytef *)malloc((size_t)(SRC_LEN + 16));
    memset(out, 0, (size_t)(SRC_LEN + 16));
    istrm.next_out = out;
    istrm.avail_out = (uInt)(SRC_LEN + 16);
    inflate(&istrm, 4 /* Z_FINISH */);

    // step 4: Cleanup
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(out);
    // API sequence test completed successfully
    return 66;
}