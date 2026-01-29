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
//<ID> 1070
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 1024UL;
    const uLong EXTRA_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    Bytef *extra = (Bytef *)malloc((size_t)EXTRA_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    memset(extra, 'B', (size_t)EXTRA_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN + EXTRA_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)(ORIG_LEN + EXTRA_LEN));
    memset(decomp, 0, (size_t)(ORIG_LEN + EXTRA_LEN));
    uLong ad1 = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong ad2 = adler32(0UL, extra, (uInt)EXTRA_LEN);
    uLong combined_ad = adler32_combine64(ad1, ad2, (off64_t)EXTRA_LEN);

    // step 2: Initialize and perform compression with deflateInit_ -> deflate -> deflateEnd
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
    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, perform inflation, and finalize inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)(ORIG_LEN + EXTRA_LEN);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Use inflateBackInit_/inflateBackEnd for alternate inflate flow, write/read gzip file, cleanup
    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char *window = (unsigned char *)malloc(32768U);
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, decomp, (unsigned int)ORIG_LEN);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzclose_r(gr);
    free(window);
    free(orig);
    free(extra);
    free(comp);
    free(decomp);
    (void)combined_ad;
    (void)ad1;
    (void)ad2;
    (void)ver;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}