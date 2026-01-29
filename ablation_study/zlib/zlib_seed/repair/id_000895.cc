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
//<ID> 895
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test payload for deflateCopy and gz operations.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t first_crc = crc_table ? crc_table[0] : 0;
    z_stream src;
    z_stream dst;
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    int init_ret = deflateInit_(&src, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    src.next_in = (Bytef *)source;
    src.avail_in = (uInt)source_len;
    uLong out_bound = deflateBound(&src, source_len);
    Bytef *out_buf_src = (Bytef *)malloc((size_t)out_bound);
    Bytef *out_buf_dst = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf_src, 0, (size_t)out_bound);
    memset(out_buf_dst, 0, (size_t)out_bound);
    src.next_out = out_buf_src;
    src.avail_out = (uInt)out_bound;

    // step 3: Operate
    int copy_ret = deflateCopy(&dst, &src);
    dst.next_out = out_buf_dst;
    dst.avail_out = (uInt)out_bound;
    int def_src_ret = deflate(&src, 4);
    uLong comp_len_src = src.total_out;
    int def_dst_ret = deflate(&dst, 4);
    uLong comp_len_dst = dst.total_out;
    gzFile gzf = gzopen64("zlib_api_seq_tmp.gz", "wb");
    int gz_put_ret = gzputs(gzf, "gzputs: compressed and original data written.");
    off64_t gzf_off = gzoffset64(gzf);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int end_src = deflateEnd(&src);
    int end_dst = deflateEnd(&dst);
    free(out_buf_src);
    free(out_buf_dst);
    (void)version;
    (void)init_ret;
    (void)first_crc;
    (void)out_bound;
    (void)copy_ret;
    (void)def_src_ret;
    (void)comp_len_src;
    (void)def_dst_ret;
    (void)comp_len_dst;
    (void)gz_put_ret;
    (void)gzf_off;
    (void)gz_cl;
    (void)end_src;
    (void)end_dst;
    // API sequence test completed successfully
    return 66;
}