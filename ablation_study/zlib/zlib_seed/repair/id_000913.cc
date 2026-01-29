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
//<ID> 913
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef source[] = "Zlib API sequence test payload for meaningful data flow.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);

    // step 2: Configure
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)source);

    // step 3: Operate
    off_t offset = gzoffset(gzf);
    int gz_cl_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_table_entry = crc_tbl ? crc_tbl[0] : (z_crc_t)0;
    uLong combined_value = ((uLong)first_table_entry) ^ (uLong)comp_len;
    free(comp_buf);
    (void)comp_ret;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl_ret;
    (void)crc_tbl;
    (void)first_table_entry;
    (void)combined_value;
    // API sequence test completed successfully
    return 66;
}