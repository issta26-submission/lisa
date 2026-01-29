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
//<ID> 908
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
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    uLong bound = deflateBound(&dstrm, ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    // step 2: Compress the data and finalize deflate
    deflate(&dstrm, 4);
    uLong compressed_size = (uLong)dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, prime the bit buffer, perform inflate, and init inflate-back
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compressed_size;
    Bytef *out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out, 0, (size_t)ORIG_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 1, 1);
    inflate(&istrm, 0);

    z_stream bstrm;
    memset(&bstrm, 0, sizeof(bstrm));
    bstrm.zalloc = NULL;
    bstrm.zfree = NULL;
    bstrm.opaque = NULL;
    unsigned char window[1];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&bstrm, 15, window, ver, (int)sizeof(z_stream));
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 4: Cleanup
    inflateEnd(&istrm);
    inflateBackEnd(&bstrm);
    free(orig);
    free(comp);
    free(out);
    (void)ver;
    (void)compressed_size;
    // API sequence test completed successfully
    return 66;
}