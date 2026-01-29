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
//<ID> 1738
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence: compress2 -> inflateInit_ -> inflateCopy -> inflate -> gzwrite/gzflush/gzeof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLong comp_len = bound;
    int comp_ret = compress2(comp_buf, (uLongf *)&comp_len, src, src_len, 6);

    // step 2: Configure inflate streams and gzFile
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf1 = (Bytef *)malloc((size_t)(src_len + 8));
    memset(decomp_buf1, 0, (int)(src_len + 8));
    inf_strm.next_out = decomp_buf1;
    inf_strm.avail_out = (uInt)(src_len + 8);
    z_stream inf_copy;
    memset(&inf_copy, 0, (int)sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_strm);
    Bytef *decomp_buf2 = (Bytef *)malloc((size_t)(src_len + 8));
    memset(decomp_buf2, 0, (int)(src_len + 8));
    inf_copy.next_out = decomp_buf2;
    inf_copy.avail_out = (uInt)(src_len + 8);

    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gf_flush_ret = gzflush(gzf, 0);
    int eof_flag_write = gzeof(gzf);
    int gzclose_w_ret = gzclose(gzf);

    // step 3: Operate (inflate original and the copy)
    int inf_ret = inflate(&inf_strm, 0);
    int inf_copy_ret = inflate(&inf_copy, 0);
    int inf_end_ret = inflateEnd(&inf_strm);
    int inf_copy_end_ret = inflateEnd(&inf_copy);

    // step 4: Validate & Cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef read_buf[128];
    memset(read_buf, 0, (int)sizeof(read_buf));
    int gzread_ret = gzread(gzf_r, read_buf, (unsigned int)sizeof(read_buf));
    int eof_flag_read = gzeof(gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);

    free(comp_buf);
    free(decomp_buf1);
    free(decomp_buf2);

    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)gw_ret;
    (void)gf_flush_ret;
    (void)eof_flag_write;
    (void)gzclose_w_ret;
    (void)inf_ret;
    (void)inf_copy_ret;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    (void)gzread_ret;
    (void)eof_flag_read;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}