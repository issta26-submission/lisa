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
//<ID> 1732
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: compress with compress2 -> init inflate -> inflateCopy -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Setup inflate streams and copy
    z_stream inf_src;
    z_stream inf_copy;
    memset(&inf_src, 0, (int)sizeof(inf_src));
    memset(&inf_copy, 0, (int)sizeof(inf_copy));
    int inf_init_ret = inflateInit_(&inf_src, ver, (int)sizeof(z_stream));
    inf_src.next_in = comp_buf;
    inf_src.avail_in = (uInt)comp_len;
    int copy_ret = inflateCopy(&inf_copy, &inf_src);

    // step 3: Operate (inflate on both original and copy) and gz operations
    Bytef *decomp_buf1 = (Bytef *)malloc((size_t)(src_len + 16));
    Bytef *decomp_buf2 = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf1, 0, (int)(src_len + 16));
    memset(decomp_buf2, 0, (int)(src_len + 16));
    inf_copy.next_out = decomp_buf1;
    inf_copy.avail_out = (uInt)(src_len + 16);
    int inf_copy_ret = inflate(&inf_copy, 0);
    inf_src.next_out = decomp_buf2;
    inf_src.avail_out = (uInt)(src_len + 16);
    int inf_src_ret = inflate(&inf_src, 0);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gf_ret = gzflush(gzf, 2);
    int eof_flag = gzeof(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int inf_copy_end = inflateEnd(&inf_copy);
    int inf_src_end = inflateEnd(&inf_src);
    free(comp_buf);
    free(decomp_buf1);
    free(decomp_buf2);
    (void)ver;
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_copy_ret;
    (void)inf_src_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)eof_flag;
    (void)gzclose_ret;
    (void)inf_copy_end;
    (void)inf_src_end;
    // API sequence test completed successfully
    return 66;
}