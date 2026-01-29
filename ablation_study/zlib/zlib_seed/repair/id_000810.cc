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
//<ID> 810
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const char msg[] = "Zlib API sequence sample payload";
    uLong msg_len = (uLong)(sizeof(msg) - 1);
    uLong bound = compressBound(msg_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, (const Bytef *)msg, msg_len, 6);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)msg_len + 16);
    memset(decomp_buf, 0, (size_t)msg_len + 16);
    uLongf decomp_len = (uLongf)msg_len;
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, (uLong)comp_len);

    // step 2: Configure
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char *window_buf = (unsigned char *)malloc(32768);
    memset(window_buf, 0, 32768);
    int infl_back_init_ret = inflateBackInit_(&back_strm, 15, window_buf, version, (int)sizeof(z_stream));
    const char *gzname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(gzname, "wb");
    int gz_set_ret = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, decomp_buf, (unsigned int)decomp_len);
    int gz_cl_w = gzclose(gzf_w);

    // step 3: Operate
    gzFile gzf_r = gzopen(gzname, "rb");
    unsigned char read_buf[512];
    memset(read_buf, 0, sizeof(read_buf));
    int gz_rd = gzread(gzf_r, (voidp)read_buf, (unsigned int)sizeof(read_buf));
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    int infl_end = inflateEnd(&back_strm);
    free(comp_buf);
    free(decomp_buf);
    free(window_buf);
    (void)version;
    (void)comp_ret;
    (void)uncomp_ret;
    (void)comp_len;
    (void)decomp_len;
    (void)infl_back_init_ret;
    (void)gz_set_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)infl_end;
    // API sequence test completed successfully
    return 66;
}