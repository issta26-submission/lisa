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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char source[] = "Zlib API sequence test payload for compression and gz operations";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, (const Bytef *)source, src_len, 6);

    // step 2: Configure
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, source);
    off_t offset = gzoffset(gzf);

    // step 3: Operate / Validate
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    uLong crc_val = crc32(0UL, (const Bytef *)source, (uInt)src_len);

    // step 4: Cleanup / Final validation
    int gz_cl_ret = gzclose(gzf);
    free(comp_buf);
    (void)comp_ret;
    (void)comp_len;
    (void)gz_put_ret;
    (void)offset;
    (void)first_crc;
    (void)crc_val;
    (void)gz_cl_ret;
    // API sequence test completed successfully
    return 66;
}