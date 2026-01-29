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
//<ID> 1255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and buffers
    const uLong SRC_LEN = 32UL;
    const uInt COMP_BUF_SIZE = 128U;
    const uInt OUT_BUF_SIZE = 128U;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)COMP_BUF_SIZE);
    Bytef *out = (Bytef *)malloc((size_t)OUT_BUF_SIZE);
    memset(src, 'A', (size_t)SRC_LEN);
    memset(comp, 0, (size_t)COMP_BUF_SIZE);
    memset(out, 0, (size_t)OUT_BUF_SIZE);

    // step 2: Initialize deflate stream, compress source, then reset keep
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = COMP_BUF_SIZE;
    deflate(&dstrm, 4);
    uInt comp_len = COMP_BUF_SIZE - dstrm.avail_out;
    deflateResetKeep(&dstrm);

    // step 3: Initialize inflate stream, inflate compressed data and validate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = comp_len;
    istrm.next_out = out;
    istrm.avail_out = OUT_BUF_SIZE;
    inflate(&istrm, 0);
    inflateValidate(&istrm, 15);

    // step 4: Cleanup streams and buffers
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(out);
    // API sequence test completed successfully
    return 66;
}