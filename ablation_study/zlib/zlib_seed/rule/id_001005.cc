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
//<ID> 1005
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data and then uncompress using uncompress2
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    uLong source_len = (uLong)comp_len;
    uLongf dest_len = (uLongf)ORIG_LEN;
    uncompress2(decomp, &dest_len, comp, &source_len);

    // step 3: Initialize an inflate stream, prime it, then end it
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp_via_inflate = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp_via_inflate, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp_via_inflate;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 0, 0);
    inflateEnd(&istrm);

    // step 4: Use gzFile to demonstrate gzclearerr, cleanup and finalize
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gzf, (int)orig[0]);
    gzclearerr(gzf);
    gzclose(gzf);
    free(orig);
    free(comp);
    free(decomp);
    free(decomp_via_inflate);
    (void)ver;
    (void)dest_len;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}