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
//<ID> 1002
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (initialize)
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
    uLong source_len = (uLong)comp_len;

    // step 2: Compress data and configure inflate stream (configure)
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit2_(&istrm, 15, ver, (int)sizeof(z_stream));
    inflatePrime(&istrm, 0, 0);

    // step 3: Operate — use uncompress2 to decompress and use a gzip file to exercise gzclearerr (operate)
    uLongf decomp_len = (uLongf)ORIG_LEN;
    uncompress2(decomp, &decomp_len, comp, &source_len);
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gf, (int)orig[0]);
    gzclearerr(gf);
    gzclose(gf);

    // step 4: Validate & cleanup (validate + cleanup)
    uLong crc_orig = crc32(0L, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0L, decomp, (uInt)decomp_len);
    (void)crc_orig;
    (void)crc_decomp;
    inflateEnd(&istrm);
    free(orig);
    free(comp);
    free(decomp);
    (void)ver;
    // API sequence test completed successfully
    return 66;
}