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
//<ID> 1023
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'X', (size_t)ORIG_LEN);
    uLong flags = zlibCompileFlags();
    uLong crc_orig = crc32_z(0L, orig, (z_size_t)ORIG_LEN);

    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    uLong crc_comp = crc32_z(0L, comp, (z_size_t)comp_len);

    const char *fname = "zlib_fuzz_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    gzbuffer(gw, 8192);
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 1);
    gzclose(gw);

    gzFile gr = gzopen(fname, "rb");
    int ungot = gzungetc('A', gr);
    char linebuf[32];
    char *res = gzgets(gr, linebuf, (int)sizeof(linebuf));
    gzclose(gr);

    free(orig);
    free(comp);
    (void)flags;
    (void)crc_orig;
    (void)crc_comp;
    (void)comp_len;
    (void)bound;
    (void)ungot;
    (void)res;
    return 66;
}