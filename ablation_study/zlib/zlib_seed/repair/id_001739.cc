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
//<ID> 1739
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress -> inflateInit_ -> inflateCopy -> inflate -> gzwrite/gzflush/gzeof";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    z_stream inf_src;
    z_stream inf_copy;
    memset(&inf_src, 0, (int)sizeof(inf_src));
    memset(&inf_copy, 0, (int)sizeof(inf_copy));

    // step 2: Setup (compress input and prepare inflate streams)
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    inf_src.next_in = comp_buf;
    inf_src.avail_in = (uInt)comp_len;
    inf_src.next_out = decomp_buf;
    inf_src.avail_out = (uInt)(src_len + 16);
    int inf_init_ret = inflateInit_(&inf_src, ver, (int)sizeof(z_stream));

    // step 3: Operate (copy inflate state and perform inflation twice)
    int copy_ret = inflateCopy(&inf_copy, &inf_src);
    int inf_ret1 = inflate(&inf_src, 0);
    int inf_ret2 = inflate(&inf_copy, 0);
    int inf_end_ret1 = inflateEnd(&inf_src);
    int inf_end_ret2 = inflateEnd(&inf_copy);

    // step 4: Validate & Cleanup (write decompressed data to gzFile, flush, check EOF, cleanup)
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)src_len);
    int gf_ret = gzflush(gzf, 2);
    int eof_flag = gzeof(gzf);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret1;
    (void)inf_ret2;
    (void)inf_end_ret1;
    (void)inf_end_ret2;
    (void)gw_ret;
    (void)gf_ret;
    (void)eof_flag;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}