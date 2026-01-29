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
//<ID> 889
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress data and initialize inflate source stream
    compress(comp, &compLen, orig, ORIG_LEN);
    z_stream src;
    memset(&src, 0, sizeof(src));
    src.zalloc = NULL;
    src.zfree = NULL;
    src.opaque = NULL;
    inflateInit_(&src, ver, (int)sizeof(z_stream));
    src.next_in = comp;
    src.avail_in = (uInt)compLen;
    Bytef *out1 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out1, 0, (size_t)ORIG_LEN);
    src.next_out = out1;
    src.avail_out = (uInt)ORIG_LEN;

    // step 3: Copy inflate stream and perform inflate on both streams; then use gzFile APIs
    z_stream copy;
    memset(&copy, 0, sizeof(copy));
    inflateCopy(&copy, &src);
    Bytef *out2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out2, 0, (size_t)ORIG_LEN);
    copy.next_out = out2;
    copy.avail_out = (uInt)ORIG_LEN;
    inflate(&src, 0);
    inflate(&copy, 0);
    gzFile gf = gzopen("/dev/null", "rb");
    gzbuffer(gf, 4096);
    int ch = gzgetc(gf);
    gzclose(gf);

    // step 4: Cleanup
    inflateEnd(&src);
    inflateEnd(&copy);
    free(orig);
    free(comp);
    free(out1);
    free(out2);
    (void)ver;
    (void)bound;
    (void)compLen;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}