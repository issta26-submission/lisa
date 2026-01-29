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
//<ID> 1074
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, (size_t)32768);

    // step 2: Initialize and perform compression (deflate) -> finalize
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

    // step 3: Inflate using standard inflate -> finalize; also demonstrate inflateBackInit_ -> inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&bstrm);

    // step 4: Write compressed data to gzip file, read it back, combine checksums, cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, comp, (unsigned int)comp_size);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzread(gr, decomp, (unsigned int)ORIG_LEN);
    int rc_close_r = gzclose_r(gr);
    uLong ad1 = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong ad2 = adler32(0L, decomp, (uInt)ORIG_LEN);
    uLong combined = adler32_combine64(ad1, ad2, (long long)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    free(window);
    (void)ver;
    (void)comp_size;
    (void)rc_close_r;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}