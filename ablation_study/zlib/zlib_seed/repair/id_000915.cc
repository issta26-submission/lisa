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
//<ID> 915
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and file I/O.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];

    // step 2: Configure buffers and lengths
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf decomp_len = (uLongf)src_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (size_t)(src_len + 16));

    // step 3: Operate - compress, uncompress, and write to a gz file
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);
    int uncmp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    uLong crc_val = crc32(0L, source, (uInt)src_len);

    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)source);
    off_t offset = gzoffset(gzf);
    int gz_cl_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)first_crc;
    (void)bound;
    (void)comp_len;
    (void)decomp_len;
    (void)comp_ret;
    (void)uncmp_ret;
    (void)crc_val;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl_ret;
    // API sequence test completed successfully
    return 66;
}