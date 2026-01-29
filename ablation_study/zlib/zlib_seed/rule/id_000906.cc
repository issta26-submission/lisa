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
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers and compress with deflate
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
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
    uLong compressed_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 2: Initialize inflate stream, prime it, then perform inflate
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
    inflatePrime(&istrm, 0, 0);
    inflate(&istrm, 0);

    // step 3: Initialize inflate-back stream and query compile flags for validation
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[1];
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    uLong flags = zlibCompileFlags();
    (void)flags;

    // step 4: Cleanup
    inflateEnd(&istrm);
    inflateBackEnd(&backstrm);
    free(orig);
    free(comp);
    free(out);
    (void)ver;
    (void)compressed_size;
    // API sequence test completed successfully
    return 66;
}