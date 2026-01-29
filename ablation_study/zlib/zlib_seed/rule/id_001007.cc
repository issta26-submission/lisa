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
//<ID> 1007
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
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);

    // step 2: Compress original data and uncompress using uncompress2
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    uLong sourceLen = (uLong)comp_len;
    uLongf destLen = (uLongf)ORIG_LEN;
    uncompress2(decomp, &destLen, comp, &sourceLen);

    // step 3: Initialize inflate with specific window bits, prime bits, inflate and cleanup inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = decomp2;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflatePrime(&istrm, 1, 1);
    inflate(&istrm, 0);
    inflateEnd(&istrm);

    // step 4: Use gz API, clear errors, close and cleanup memory
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, orig, (unsigned int)ORIG_LEN);
    gzclearerr(gf);
    gzclose_w(gf);
    free(orig);
    free(comp);
    free(decomp);
    free(decomp2);
    (void)ver;
    (void)sourceLen;
    (void)destLen;
    // API sequence test completed successfully
    return 66;
}