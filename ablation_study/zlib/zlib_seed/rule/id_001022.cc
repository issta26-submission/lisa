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
//<ID> 1022
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    const uLong ORIG_LEN = 512UL;
    uLong flags = zlibCompileFlags();
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong crc_orig = crc32_z(0L, orig, (z_size_t)ORIG_LEN);

    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    uLongf decomp_len = (uLongf)ORIG_LEN;
    uLong sourceLen = (uLong)comp_len;
    uncompress2(decomp, &decomp_len, comp, &sourceLen);

    gzFile gw = gzopen("zlib_test_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 0);
    gzclearerr(gw);
    gzclose(gw);
    gzFile gr = gzopen("zlib_test_tmp.gz", "rb");
    gzungetc('P', gr);
    gzclearerr(gr);
    gzclose(gr);

    uLong crc_decomp = crc32_z(0L, decomp, (z_size_t)decomp_len);
    free(orig);
    free(comp);
    free(decomp);
    (void)flags;
    (void)crc_orig;
    (void)crc_decomp;
    (void)comp_len;
    (void)decomp_len;
    (void)sourceLen;
    return 66;
}