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
//<ID> 950
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
    memset(orig, 'A', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gw, comp, (unsigned int)comp_len);
    gzclose(gw);
    uLong adler_orig = adler32(0L, orig, (uInt)ORIG_LEN);

    // step 2: Initialize inflate stream, inflate compressed buffer, then reset and end
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateReset(&istrm);
    inflateEnd(&istrm);
    uLong adler_decomp = adler32(0L, decomp, (uInt)ORIG_LEN);

    // step 3: Initialize deflate stream, exercise deflateResetKeep, then end
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);
    deflateEnd(&dstrm);

    // step 4: Cleanup and finish
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    (void)adler_orig;
    (void)adler_decomp;
    // API sequence test completed successfully
    return 66;
}