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
//<ID> 1043
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
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLong comp_total = 0UL;

    // step 2: Initialize deflate, perform a full deflate, then reset and perform another deflate
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong first_out = (uLong)dstrm.total_out;
    deflateReset(&dstrm);
    dstrm.next_in = orig; /* reuse input */
    dstrm.avail_in = (uInt)(ORIG_LEN / 2);
    dstrm.next_out = comp + first_out;
    dstrm.avail_out = (uInt)(bound - first_out);
    deflate(&dstrm, 0); /* Z_NO_FLUSH */
    uLong second_out = (uLong)dstrm.total_out;
    comp_total = first_out + second_out;
    deflateEnd(&dstrm);

    // step 3: Write part of the original data to a gzip file using buffered gzFile and gzputc
    gzFile gw = gzopen("zlib_seq_tmp_write.gz", "wb");
    gzbuffer(gw, 8192U);
    gzputc(gw, (int)orig[0]);
    gzputc(gw, (int)orig[1]);
    gzputc(gw, (int)orig[2]);
    gzclose(gw);

    // step 4: Validate and cleanup
    free(orig);
    free(comp);
    (void)flags;
    (void)comp_total;
    (void)first_out;
    (void)second_out;
    // API sequence test completed successfully
    return 66;
}