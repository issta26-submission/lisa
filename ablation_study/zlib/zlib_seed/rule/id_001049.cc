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
//<ID> 1049
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong flags = zlibCompileFlags();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);

    // step 2: Initialize deflate and perform compression
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
    deflateReset(&dstrm);
    deflateEnd(&dstrm);

    // step 3: Write data to a gzip file and exercise buffering & gzputc
    gzFile gw = gzopen("zlib_api_sequence_tmp.gz", "wb");
    gzbuffer(gw, 8192);
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    if (comp_size == 0) { /* no-op to avoid unused-suppression via comp_size usage in straight-line flow */ }
    gzputc(gw, (int)comp[0]);
    gzclose(gw);

    // step 4: Cleanup
    free(orig);
    free(comp);
    (void)ver;
    (void)flags;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}