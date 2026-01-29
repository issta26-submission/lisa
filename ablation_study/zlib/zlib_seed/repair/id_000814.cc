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
//<ID> 814
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[128];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)plain_len);
    memset(decomp_buf, 0, (size_t)plain_len);
    uLongf decomp_len = (uLongf)plain_len;
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, (uLong)comp_len);

    // step 2: Configure
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen64(fname, "wb");
    int gz_set_ret = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, plain, (unsigned int)plain_len);
    int gz_cl_w = gzclose(gzf_w);

    // step 3: Operate
    gzFile gzf_r = gzopen64(fname, "rb");
    Bytef read_buf[256];
    memset(read_buf, 0, sizeof(read_buf));
    int gz_rd = gzread(gzf_r, (voidp)read_buf, (unsigned int)sizeof(read_buf));
    int gz_cl_r = gzclose(gzf_r);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int infl_back_init_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate / Cleanup
    int infl_end_ret = inflateEnd(&strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)comp_ret;
    (void)uncomp_ret;
    (void)decomp_len;
    (void)comp_len;
    (void)gz_set_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)infl_back_init_ret;
    (void)infl_end_ret;
    (void)fname;
    // API sequence test completed successfully
    return 66;
}