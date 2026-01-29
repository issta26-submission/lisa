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
//<ID> 1620
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
    int comp_ret = compress2(comp_buf, &comp_buf_len, src, src_len, 6);

    z_stream backStrm;
    memset(&backStrm, 0, (int)sizeof(backStrm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int ibi_ret = inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gf_ret = gzflush(gzf, 0);

    // step 3: Operate
    int ir_ret = inflateReset(&backStrm);
    Bytef *dst_buf = (Bytef *)malloc((size_t)src_len);
    memset(dst_buf, 0, (int)src_len);
    uLongf dst_len = src_len;
    uLong src_len_copy = (uLong)comp_buf_len;
    int uncomp_ret = uncompress2(dst_buf, &dst_len, comp_buf, &src_len_copy);

    // step 4: Validate & Cleanup
    int gclose_ret = gzclose(gzf);
    int ib_end_ret = inflateBackEnd(&backStrm);
    free(window);
    free(comp_buf);
    free(dst_buf);

    (void)ver;
    (void)comp_ret;
    (void)ibi_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)ir_ret;
    (void)uncomp_ret;
    (void)gclose_ret;
    (void)ib_end_ret;
    // API sequence test completed successfully
    return 66;
}