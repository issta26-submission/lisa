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
//<ID> 815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const char msg[] = "Sample data for zlib usage";
    uLong msg_len = (uLong)(sizeof(msg) - 1);
    uLong bound = compressBound(msg_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, (const Bytef *)msg, msg_len, 6);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int back_init_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));
    const char fname[] = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int setparams_ret = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, (voidp)msg, (unsigned int)msg_len);
    int gz_cl_w = gzclose(gzf_w);

    // step 3: Operate
    gzFile gzf_r = gzopen(fname, "rb");
    unsigned char read_buf[128];
    memset(read_buf, 0, sizeof(read_buf));
    int gz_rd = gzread(gzf_r, (voidp)read_buf, (unsigned int)sizeof(read_buf));
    int gz_cl_r = gzclose(gzf_r);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(msg_len + 16));
    memset(decomp_buf, 0, (size_t)(msg_len + 16));
    uLongf decomp_len = (uLongf)(msg_len + 16);
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, (uLong)comp_len);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)comp_ret;
    (void)bound;
    (void)comp_len;
    (void)back_init_ret;
    (void)setparams_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)decomp_len;
    (void)uncomp_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}