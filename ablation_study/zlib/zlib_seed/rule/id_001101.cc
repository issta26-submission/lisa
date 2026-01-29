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
//<ID> 1101
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & buffers (Initialize)
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Initialize and configure streams (Configure)
    z_stream cstrm;
    memset(&cstrm, 0, sizeof(cstrm));
    cstrm.zalloc = NULL;
    cstrm.zfree = NULL;
    cstrm.opaque = NULL;
    deflateInit_(&cstrm, 6, ver, (int)sizeof(z_stream));
    cstrm.next_in = orig;
    cstrm.avail_in = (uInt)ORIG_LEN;
    cstrm.next_out = comp;
    cstrm.avail_out = (uInt)bound;

    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, 1 << 15);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Operate and simple validation (Operate -> Validate)
    deflate(&cstrm, Z_FINISH);
    uLong comp_size = cstrm.total_out;
    inflateReset(&bstrm);
    gzFile gf = gzopen("test_zlib_api_sequence_out.gz", "wb");
    gzwrite(gf, comp, (unsigned int)comp_size);
    gzclose(gf);

    // step 4: Cleanup (Cleanup)
    deflateEnd(&cstrm);
    inflateBackEnd(&bstrm);
    free(window);
    free(orig);
    free(comp);
    (void)ver;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}