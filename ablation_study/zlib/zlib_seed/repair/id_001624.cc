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
//<ID> 1624
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: inflateBackInit_ + gzflush + inflateReset + uncompress2 usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLongf comp_buf_len = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_buf_len);
    memset(comp_buf, 0, (int)comp_buf_len);

    // step 2: Configure
    int comp_ret = compress2(comp_buf, &comp_buf_len, src, src_len, 6);
    uLong comp_src_len = (uLong)comp_buf_len;
    uLongf out_buf_len = (uLongf)(src_len + 16);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (int)out_buf_len);

    // step 3: Operate
    int uncmp_ret = uncompress2(out_buf, &out_buf_len, comp_buf, &comp_src_len);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, out_buf, (unsigned int)out_buf_len);
    int gzflush_ret = gzflush(gzf, 2);

    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int infl_init_ret = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));
    int infl_reset_ret = inflateReset(&strm);

    // step 4: Validate & Cleanup
    int infl_back_end_ret = inflateBackEnd(&strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(out_buf);
    free(window);

    (void)ver;
    (void)comp_ret;
    (void)uncmp_ret;
    (void)gw_ret;
    (void)gzflush_ret;
    (void)infl_init_ret;
    (void)infl_reset_ret;
    (void)infl_back_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}