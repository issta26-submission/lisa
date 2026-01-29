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
//<ID> 1042
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

    // step 2: Initialize deflate and perform initial compression
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    const char *ver = zlibVersion();
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)ORIG_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong first_comp_size = (uLong)dstrm.total_out;

    // step 3: Reset stream, do a small additional compression, then write original to a gzip file using buffering and gzputc
    deflateReset(&dstrm);
    dstrm.next_in = orig;
    dstrm.avail_in = (uInt)1;
    dstrm.next_out = comp + first_comp_size;
    dstrm.avail_out = (uInt)(bound - first_comp_size);
    deflate(&dstrm, 4); /* Z_FINISH for the small chunk */
    uLong second_comp_size = (uLong)dstrm.total_out;
    uLong total_comp_size = first_comp_size + second_comp_size;
    gzFile gw = gzopen("zlib_seq_tmp.gz", "wb");
    gzbuffer(gw, 8192U);
    gzputc(gw, (int)'A');
    gzwrite(gw, orig + 1, (unsigned int)(ORIG_LEN - 1));
    gzclose(gw);

    // step 4: Cleanup and finalize
    deflateEnd(&dstrm);
    free(orig);
    free(comp);
    (void)flags;
    (void)ver;
    (void)first_comp_size;
    (void)second_comp_size;
    (void)total_comp_size;
    // API sequence test completed successfully
    return 66;
}