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
//<ID> 1028
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
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);

    // step 2: Compress data and write original to a gzip file (use filename to avoid FD hang)
    compress2(comp, &comp_len, orig, (uLong)ORIG_LEN, 6);
    gzFile gw = gzopen("zlib_seq_tmp.gz", "wb");
    gzwrite(gw, orig, (unsigned int)ORIG_LEN);
    gzflush(gw, 0);
    gzclose(gw);

    // step 3: Decompress the compressed buffer and exercise gzungetc on a reader stream
    uLongf decomp_len = (uLongf)ORIG_LEN;
    uncompress(decomp, &decomp_len, comp, (uLong)comp_len);
    gzFile gr = gzopen("zlib_seq_tmp.gz", "rb");
    gzungetc((int)'Z', gr);
    gzclose(gr);
    uLong crc_decomp = crc32_z(0UL, decomp, (z_size_t)decomp_len);

    // step 4: Cleanup and finalize
    free(orig);
    free(comp);
    free(decomp);
    (void)flags;
    (void)crc_orig;
    (void)crc_decomp;
    (void)comp_len;
    (void)decomp_len;
    // API sequence test completed successfully
    return 66;
}