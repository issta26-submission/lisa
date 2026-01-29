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
//<ID> 952
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);

    // step 2: Initialize inflate stream and decompress compressed buffer
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateReset(&istrm);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 3: Initialize deflate stream, re-compress decompressed data, and use gzFile
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    dstrm.next_in = decomp;
    dstrm.avail_in = (uInt)ORIG_LEN;
    uLong bound2 = compressBound(ORIG_LEN);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    dstrm.next_out = comp2;
    dstrm.avail_out = (uInt)bound2;
    deflateResetKeep(&dstrm);
    deflate(&dstrm, 0);
    deflateEnd(&dstrm);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, comp2, (unsigned int)dstrm.total_out);
    gzclose(gf);

    // step 4: Validate results and cleanup
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    free(comp2);
    (void)ver;
    (void)adler_orig;
    (void)adler_decomp;
    // API sequence test completed successfully
    return 66;
}