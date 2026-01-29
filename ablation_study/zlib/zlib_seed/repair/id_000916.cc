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
//<ID> 916
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const z_crc_t *crc_tbl = get_crc_table();
    const Bytef source[] = "Sample zlib payload for API sequence test.";
    uLong src_len = (uLong)(sizeof(source) - 1);

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress(comp_buf, &comp_len, source, src_len);

    // step 3: Operate
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)source);
    off_t offset = gzoffset(gzf);
    int gz_cl_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (size_t)(src_len + 16));
    uLongf decomp_len = (uLongf)(src_len + 16);
    int uncmp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    z_crc_t first_crc = crc_tbl[0];
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)comp_ret;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl_ret;
    (void)uncmp_ret;
    (void)first_crc;
    // API sequence test completed successfully
    return 66;
}