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
//<ID> 1128
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers
    const uLong ORIG_CHARS = 11UL; /* "Hello zlib!" is 11 chars */
    Bytef *orig = (Bytef *)malloc((size_t)(ORIG_CHARS + 1));
    memcpy(orig, "Hello zlib!", (size_t)ORIG_CHARS);
    orig[ORIG_CHARS] = 0;
    const char *ver = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;

    // step 2: Compress original data using deflate
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)(ORIG_CHARS + 1);
    uLong bound = deflateBound(&dstrm, (uLong)(ORIG_CHARS + 1));
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, Z_FINISH);
    uLong comp_size = dstrm.total_out;

    // step 3: Initialize inflate, reset window size, perform inflate, then end inflate
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)(ORIG_CHARS + 1));
    memset(decomp, 0, (size_t)(ORIG_CHARS + 1));
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)(ORIG_CHARS + 1);
    inflateReset2(&istrm, 15);
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Write decompressed string to a gzip file and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputs(gf, (const char *)decomp);
    gzclose(gf);
    deflateEnd(&dstrm);

    free(orig);
    free(comp);
    free(decomp);

    // API sequence test completed successfully
    return 66;
}