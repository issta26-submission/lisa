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
//<ID> 1731
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & prepare data
    const Bytef src[] = "zlib API sequence: compress -> write gz -> read gz -> inflate (with inflateCopy)";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Configure inflate streams and copies
    z_stream inf_strm;
    z_stream inf_copy;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    memset(&inf_copy, 0, (int)sizeof(inf_copy));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    int inf_copy_ret = inflateCopy(&inf_copy, &inf_strm);
    Bytef *decomp_buf1 = (Bytef *)malloc((size_t)(src_len + 8));
    Bytef *decomp_buf2 = (Bytef *)malloc((size_t)(src_len + 8));
    memset(decomp_buf1, 0, (int)(src_len + 8));
    memset(decomp_buf2, 0, (int)(src_len + 8));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf1;
    inf_strm.avail_out = (uInt)(src_len + 8);
    inf_copy.next_in = comp_buf;
    inf_copy.avail_in = (uInt)comp_len;
    inf_copy.next_out = decomp_buf2;
    inf_copy.avail_out = (uInt)(src_len + 8);

    // step 3: Operate: inflate both streams AND write/read gzFile (with gzflush)
    int inf_ret1 = inflate(&inf_strm, 0);
    int inf_ret2 = inflate(&inf_copy, 0);
    int inf_end_ret1 = inflateEnd(&inf_strm);
    int inf_end_ret2 = inflateEnd(&inf_copy);
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gzf_flush_ret = gzflush(gzf_w, 2);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *gz_read_out = (Bytef *)malloc((size_t)(src_len + 8));
    memset(gz_read_out, 0, (int)(src_len + 8));
    int gr_ret = gzread(gzf_r, gz_read_out, (unsigned int)(src_len + 8));
    int eof_flag = gzeof(gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);

    // step 4: Validate & Cleanup
    (void)ver;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_copy_ret;
    (void)inf_ret1;
    (void)inf_ret2;
    (void)inf_end_ret1;
    (void)inf_end_ret2;
    (void)gw_ret;
    (void)gzf_flush_ret;
    (void)gzclose_w_ret;
    (void)gr_ret;
    (void)eof_flag;
    (void)gzclose_r_ret;
    free(comp_buf);
    free(decomp_buf1);
    free(decomp_buf2);
    free(gz_read_out);
    // API sequence test completed successfully
    return 66;
}