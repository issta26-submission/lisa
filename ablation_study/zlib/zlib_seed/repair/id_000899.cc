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
//<ID> 899
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Sample payload for zlib API sequence test";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream strm1;
    memset(&strm1, 0, sizeof(strm1));
    int init_ret = deflateInit_(&strm1, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    strm1.next_in = (Bytef *)source;
    strm1.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&strm1, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    strm1.next_out = comp_buf;
    strm1.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&strm1, 4);
    uLong comp_len = strm1.total_out;
    z_stream strm2;
    memset(&strm2, 0, sizeof(strm2));
    int copy_ret = deflateCopy(&strm2, &strm1);
    int end2_ret = deflateEnd(&strm2);
    int end1_ret = deflateEnd(&strm1);

    // step 4: Validate / Cleanup
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, (const char *)source);
    off64_t offset = gzoffset64(gzf);
    int gz_cl_ret = gzclose(gzf);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)end2_ret;
    (void)end1_ret;
    (void)gz_put_ret;
    (void)offset;
    (void)gz_cl_ret;
    (void)first_crc;
    // API sequence test completed successfully
    return 66;
}