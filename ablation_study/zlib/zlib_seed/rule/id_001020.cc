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
//<ID> 1020
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const uLong ORIG_LEN = 512UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'Z', (size_t)ORIG_LEN);
    uLong flags = zlibCompileFlags();
    uLong crc_orig = crc32_z(0UL, orig, (z_size_t)ORIG_LEN);

    // step 2: Compress data
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);

    // step 3: Write compressed data to a file via file descriptor using gzdopen
    FILE *fw = fopen("test_zlib_api_sequence_tmp.gz", "wb");
    int fdw = fileno(fw);
    gzFile gw = gzdopen(fdw, "wb");
    gzwrite(gw, comp, (unsigned int)comp_len);
    gzflush(gw, 0);
    gzclose_w(gw);
    fclose(fw);

    // step 4: Reopen file for reading, use gzungetc, read back and validate, cleanup
    FILE *fr = fopen("test_zlib_api_sequence_tmp.gz", "rb");
    int fdr = fileno(fr);
    gzFile gr = gzdopen(fdr, "rb");
    gzungetc('X', gr);
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    gzread(gr, decomp, (unsigned int)ORIG_LEN);
    gzclose_r(gr);
    fclose(fr);
    uLong crc_decomp = crc32_z(0UL, decomp, (z_size_t)ORIG_LEN);

    remove("test_zlib_api_sequence_tmp.gz");
    free(orig);
    free(comp);
    free(decomp);
    (void)flags;
    (void)crc_orig;
    (void)crc_decomp;
    (void)comp_len;
    return 66;
    /* API sequence test completed successfully */
}