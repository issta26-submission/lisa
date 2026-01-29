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
//<ID> 1730
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence: compress2 -> inflateInit_ -> inflateCopy -> inflate -> gzwrite/gzflush/gzeof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)src_len);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Configure
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gf_ret = gzflush(gzf_w, 0);
    int gzclose_w = gzclose(gzf_w);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(decomp_buf, 0, (int)(src_len + 1));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 1);
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = inflateCopy(&copy_strm, &inf_strm);

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 0);
    int inf_ret_copy = inflate(&copy_strm, 0);
    int inf_end_ret = inflateEnd(&inf_strm);
    int copy_end_ret = inflateEnd(&copy_strm);

    // step 4: Validate & Cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int read_bytes = gzread(gzf_r, read_buf, (unsigned int)(src_len + 1));
    int eof_flag = gzeof(gzf_r);
    int gzclose_r = gzclose(gzf_r);
    uLong crc_after = crc32(0UL, decomp_buf, (uInt)src_len);
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    (void)ver;
    (void)crc_before;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_w;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)inf_ret_copy;
    (void)inf_end_ret;
    (void)copy_end_ret;
    (void)read_bytes;
    (void)eof_flag;
    (void)gzclose_r;
    (void)crc_after;
    // API sequence test completed successfully
    return 66;
}