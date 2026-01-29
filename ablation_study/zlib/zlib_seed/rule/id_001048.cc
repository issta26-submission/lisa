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
//<ID> 1048
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
    uLong flags = zlibCompileFlags();
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Initialize deflate, perform compression, reset and compress remaining, finalize deflate
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
    uLong comp_first = dstrm.total_out;
    deflateReset(&dstrm);
    dstrm.next_in = orig + (ORIG_LEN / 2);
    dstrm.avail_in = (uInt)(ORIG_LEN / 2);
    dstrm.next_out = comp + comp_first;
    dstrm.avail_out = (uInt)(bound - comp_first);
    deflate(&dstrm, 4);
    uLong comp_total = dstrm.total_out;
    deflateEnd(&dstrm);

    // step 3: Write original bytes to a gzip file using gzbuffer and gzputc
    gzFile gw = gzopen("zlib_seq_tmp.gz", "wb");
    gzbuffer(gw, 4096U);
    gzputc(gw, (int)orig[0]);
    gzputc(gw, (int)orig[1]);
    gzputc(gw, (int)orig[2]);
    gzclose(gw);

    // step 4: Initialize inflate, decompress the compressed buffer, finalize inflate, cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_total;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    free(orig);
    free(comp);
    free(decomp);
    (void)flags;
    (void)ver;
    (void)comp_first;
    (void)comp_total;
    // API sequence test completed successfully
    return 66;
}