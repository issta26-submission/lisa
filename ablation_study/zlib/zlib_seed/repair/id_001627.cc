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
//<ID> 1627
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
    uLong comp_bound = compressBound(src_len);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef * comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    const char * ver = zlibVersion();

    // step 2: Configure
    int infl_back_init_ret = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));
    int infl_reset_ret = inflateReset(&strm);

    // step 3: Operate
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gzflush_ret = gzflush(gzf, 2);
    int gz_close_ret = gzclose(gzf);

    Bytef * dest_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(dest_buf, 0, (int)(src_len + 16));
    uLongf dest_len = (uLongf)(src_len + 16);
    uLong comp_src_len = (uLong)comp_len;
    int uncompress_ret = uncompress2(dest_buf, &dest_len, comp_buf, &comp_src_len);

    // step 4: Validate & Cleanup
    int infl_back_end_ret = inflateBackEnd(&strm);
    free(comp_buf);
    free(dest_buf);
    free(window);

    (void)comp_ret;
    (void)infl_back_init_ret;
    (void)infl_reset_ret;
    (void)gw_ret;
    (void)gzflush_ret;
    (void)gz_close_ret;
    (void)uncompress_ret;
    (void)infl_back_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}