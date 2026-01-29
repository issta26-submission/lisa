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
//<ID> 1175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer and metadata
    const uLong ORIG_LEN = 1024UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLongf comp_bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLongf comp_size = comp_bound;

    // step 2: Compress original data with compress()
    compress(comp, &comp_size, orig, ORIG_LEN);

    // step 3: Initialize inflate stream, mark undermine and validate, then decompress with uncompress2
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflateUndermine(&istrm, 1);
    inflateValidate(&istrm, 1);
    uLong sourceLen = (uLong)comp_size;
    uLongf destLen = (uLongf)ORIG_LEN;
    Bytef *decomp2 = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp2, 0, (size_t)ORIG_LEN);
    uncompress2(decomp2, &destLen, comp, &sourceLen);
    inflateEnd(&istrm);

    // step 4: Initialize and cleanup a deflate stream, compute checksums, cleanup buffers
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateEnd(&dstrm);
    uLong ad_orig = adler32(0UL, orig, (uInt)ORIG_LEN);
    uLong ad_decomp = adler32(0UL, decomp2, (uInt)ORIG_LEN);
    free(orig);
    free(comp);
    free(decomp);
    free(decomp2);
    (void)ad_orig;
    (void)ad_decomp;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}