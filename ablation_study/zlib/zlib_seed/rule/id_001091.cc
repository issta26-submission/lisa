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
//<ID> 1091
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();

    // step 2: Compress original data into a buffer
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(comp, &compLen, orig, ORIG_LEN);

    // step 3: Write compressed and original data via gz APIs, then read one byte and get position
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, orig, (unsigned int)ORIG_LEN);
    gzclose(gf);
    gzFile gw = gzdopen(1, "wb");
    gzwrite(gw, comp, (unsigned int)compLen);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_byte = gzgetc_(gr);
    off_t position = gztell(gr);
    gzclose_r(gr);

    // step 4: Initialize inflateBack stream, teardown, and cleanup all resources
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, 1 << 15);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);
    free(window);
    free(orig);
    free(comp);
    (void)first_byte;
    (void)position;
    (void)compLen;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}