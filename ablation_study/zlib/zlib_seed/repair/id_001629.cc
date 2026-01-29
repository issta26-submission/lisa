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
//<ID> 1629
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef plain_src[] = "zlib API sequence test: compress2 -> uncompress2 -> inflateBackInit_ -> gzflush usage";
    uLong plain_src_len = (uLong)(sizeof(plain_src) - 1);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(plain_src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_lenf = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_lenf, plain_src, plain_src_len, 6);

    // step 2: Configure
    uLongf decomp_buf_lenf = plain_src_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_buf_lenf);
    memset(decomp_buf, 0, (int)decomp_buf_lenf);
    uLong comp_source_len = (uLong)comp_lenf;
    int uncompress_ret = uncompress2(decomp_buf, &decomp_buf_lenf, comp_buf, &comp_source_len);

    // step 3: Operate
    z_stream back_strm;
    memset(&back_strm, 0, (int)sizeof(back_strm));
    unsigned char *window_buf = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window_buf, 0, (int)(1 << 15));
    int back_init_ret = inflateBackInit_(&back_strm, 15, window_buf, ver, (int)sizeof(z_stream));
    int reset_ret = inflateReset(&back_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)decomp_buf_lenf);
    int gzflush_ret = gzflush(gzf, 2);

    // step 4: Validate & Cleanup
    int back_end_ret = inflateBackEnd(&back_strm);
    int gz_close_ret = gzclose(gzf);
    free(window_buf);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_lenf;
    (void)uncompress_ret;
    (void)back_init_ret;
    (void)reset_ret;
    (void)gw_ret;
    (void)gzflush_ret;
    (void)back_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}