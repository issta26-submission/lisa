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
//<ID> 1106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    z_stream dstrm;
    z_stream istrm;
    z_stream bstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    memset(&istrm, 0, sizeof(istrm));
    memset(&bstrm, 0, sizeof(bstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;

    // step 2: Initialize deflate and perform compression (Operate)
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;

    // step 3: Initialize inflate, reset, validate, and initialize inflateBack_
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateReset(&istrm);
    inflateValidate(&istrm, 0);
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, (size_t)(1 << 15));
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 4: Write compressed data to a gzip file and cleanup resources
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_size);
    gzclose(gf);
    inflateBackEnd(&bstrm);
    inflateEnd(&istrm);
    deflateEnd(&dstrm);
    free(window);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}