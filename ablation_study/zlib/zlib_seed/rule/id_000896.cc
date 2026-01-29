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
//<ID> 896
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compression using deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    deflateEnd(&dstrm);

    // step 3: InflateBack initialization and gzFile operations
    unsigned int WINDOW_BITS = 15;
    size_t window_size = 1u << 15;
    unsigned char *window = (unsigned char *)malloc(window_size);
    memset(window, 0, window_size);
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    inflateBackInit_(&bstrm, (int)WINDOW_BITS, window, ver, (int)sizeof(z_stream));
    gzFile gf = gzopen("/dev/null", "rb");
    int ch = gzgetc_(gf);
    gzclose(gf);
    inflateBackEnd(&bstrm);

    // step 4: Cleanup
    free(orig);
    free(comp);
    free(window);
    (void)ver;
    (void)bound;
    (void)compLen;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}