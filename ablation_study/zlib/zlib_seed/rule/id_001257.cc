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
//<ID> 1257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data
    const uLong SRC_LEN = 200UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);

    // step 2: Initialize and configure deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit2_(&dstrm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);

    uLong compBound = deflateBound(&dstrm, SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)compBound;

    // step 3: Operate deflate to produce compressed data and finalize deflate
    deflate(&dstrm, 4); // Z_FINISH = 4
    uLong compSize = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 4: Initialize inflate, validate, inflate the data and cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compSize;
    Bytef *out = (Bytef *)malloc((size_t)SRC_LEN);
    memset(out, 0, (size_t)SRC_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)SRC_LEN;
    inflateValidate(&istrm, 0);
    inflate(&istrm, 4); // Z_FINISH = 4
    uLong outSize = istrm.total_out;
    (void)outSize;

    inflateEnd(&istrm);
    free(src);
    free(comp);
    free(out);
    // API sequence test completed successfully
    return 66;
}