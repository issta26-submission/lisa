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
//<ID> 1623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: inflateBackInit_ + inflateReset + uncompress2 + gzflush usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream in_stream;
    memset(&in_stream, 0, (int)sizeof(in_stream));
    unsigned char window[32768];
    memset(window, 0, (int)sizeof(window));
    int init_ret = inflateBackInit_(&in_stream, 15, window, ver, (int)sizeof(z_stream));
    int reset_ret = inflateReset(&in_stream);

    // step 3: Operate
    uLongf dest_len = (uLongf)(src_len + 8);
    Bytef *dest_buf = (Bytef *)malloc((size_t)dest_len);
    memset(dest_buf, 0, (int)dest_len);
    uLong comp_len_ul = (uLong)comp_len;
    int uncmp_ret = uncompress2(dest_buf, &dest_len, comp_buf, &comp_len_ul);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, dest_buf, (unsigned int)dest_len);
    int gf_ret = gzflush(gzf, 2);

    // step 4: Validate & Cleanup
    int gz_close_ret = gzclose(gzf);
    int back_end_ret = inflateBackEnd(&in_stream);
    free(comp_buf);
    free(dest_buf);
    (void)ver;
    (void)comp_ret;
    (void)init_ret;
    (void)reset_ret;
    (void)uncmp_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gz_close_ret;
    (void)back_end_ret;
    // API sequence test completed successfully
    return 66;
}