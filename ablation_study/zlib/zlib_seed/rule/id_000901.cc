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
//<ID> 901
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Initialize deflate and compress data
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
    uLong comp_len = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Initialize inflate, prime bit buffer, and perform inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *out = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(out, 0, (size_t)ORIG_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 3, 0);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Query compile flags, initialize inflate-back stream and cleanup
    uLong flags = zlibCompileFlags();
    z_stream ibstrm;
    memset(&ibstrm, 0, sizeof(ibstrm));
    ibstrm.zalloc = NULL;
    ibstrm.zfree = NULL;
    ibstrm.opaque = NULL;
    unsigned char window[1];
    inflateBackInit_(&ibstrm, 15, window, ver, (int)sizeof(z_stream));
    int ib_end = inflateBackEnd(&ibstrm);
    free(orig);
    free(comp);
    free(out);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)flags;
    (void)ib_end;
    // API sequence test completed successfully
    return 66;
}