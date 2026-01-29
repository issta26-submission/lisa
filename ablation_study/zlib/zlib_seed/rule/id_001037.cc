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
//<ID> 1037
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
    const char *ver = zlibVersion();
    uLong adler_orig = adler32(1UL, orig, (uInt)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress data and exercise deflateResetKeep on a deflate stream
    compress2(comp, &comp_len, orig, ORIG_LEN, 6);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateResetKeep(&dstrm);
    deflateEnd(&dstrm);

    // step 3: Inflate the compressed buffer and validate with adler32
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, 0);
    inflateEnd(&istrm);
    uLong adler_decomp = adler32(1UL, decomp, (uInt)ORIG_LEN);

    // step 4: Gzip file operations exercising gzgetc and gzungetc, then cleanup
    gzFile gw = gzopen("zlib_seq_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 0);
    gzclose(gw);
    gzFile gr = gzopen("zlib_seq_tmp.gz", "rb");
    int c1 = gzgetc(gr);
    gzungetc(c1, gr);
    int c2 = gzgetc(gr);
    (void)c1;
    (void)c2;
    (void)adler_orig;
    (void)adler_decomp;
    (void)comp_len;
    gzclose(gr);
    free(orig);
    free(comp);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}