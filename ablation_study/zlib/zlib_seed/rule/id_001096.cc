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
//<ID> 1096
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong LEN = 128UL;
    Bytef *src = (Bytef *)malloc((size_t)LEN);
    memset(src, 'A', (size_t)LEN);
    uLong bound = compressBound(LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)LEN);
    memset(decomp, 0, (size_t)LEN);
    const char *ver = zlibVersion();

    // step 2: Compress using deflate stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Inflate using inflate and exercise inflateBackInit_/inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)LEN;
    inflate(&istrm, 4);
    inflateEnd(&istrm);
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

    // step 4: Gzip file operations and gzdopen/gzgetc_/gztell usage, then cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, decomp, (unsigned int)LEN);
    off_t posw = gztell(gw);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    int ch = gzgetc_(gr);
    off_t posr = gztell(gr);
    gzclose(gr);
    gzFile gzd = gzdopen(1, "wb");
    gzwrite(gzd, decomp, (unsigned int)4);
    gzclose(gzd);

    free(src);
    free(comp);
    free(decomp);
    (void)ver;
    (void)comp_size;
    (void)posw;
    (void)posr;
    (void)ch;
    // API sequence test completed successfully
    return 66;
}