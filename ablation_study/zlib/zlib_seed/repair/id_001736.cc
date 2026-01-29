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
//<ID> 1736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: compress with compress2 -> inflate -> inflateCopy -> gzwrite/gzflush/gzeof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong checksum_before = adler32(0UL, src, (uInt)src_len);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len_u = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_u, src, src_len, 6);

    // step 2: Configure inflate streams
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len_u;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(decomp_buf, 0, (int)(src_len + 1));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 1);

    // step 3: Operate - copy inflate state and run inflate on both
    z_stream inf_copy;
    memset(&inf_copy, 0, (int)sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_strm);
    int inf_ret = inflate(&inf_strm, 0);
    int inf_copy_ret = inflate(&inf_copy, 0);

    // step 4: Validate via gzFile operations and Cleanup
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int flush_ret = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int gr_ret = gzread(gzf_r, read_buf, (unsigned int)(src_len + 1));
    int eof_flag = gzeof(gzf_r);
    int gzclose_r = gzclose(gzf_r);
    int inf_end_ret = inflateEnd(&inf_strm);
    int inf_copy_end_ret = inflateEnd(&inf_copy);
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    (void)ver;
    (void)checksum_before;
    (void)bound;
    (void)comp_ret;
    (void)comp_len_u;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)inf_copy_ret;
    (void)gw_ret;
    (void)flush_ret;
    (void)gzclose_w;
    (void)gr_ret;
    (void)eof_flag;
    (void)gzclose_r;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}