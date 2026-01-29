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
//<ID> 1044
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 128UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong flags = zlibCompileFlags();
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Initialize deflate and perform first compression
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
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size1 = dstrm.total_out;

    // step 3: Reset deflate, reuse for a second short compression, and write original to gzip file with buffering and putc
    deflateReset(&dstrm);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)(ORIG_LEN / 2);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size2 = dstrm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzbuffer(gw, 8192);
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzputc(gw, 'Z');
    gzclose(gw);

    // step 4: Cleanup and finalize
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    (void)flags;
    (void)comp_size1;
    (void)comp_size2;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}