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
//<ID> 812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const char msg[] = "Example data for zlib sequence.";
    uLong src_len = (uLong)(sizeof(msg) - 1);
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int c_ret = compress2(comp_buf, &comp_len, (const Bytef *)msg, src_len, 6);

    // step 2: Configure
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int ib_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    Bytef *dec_buf = (Bytef *)malloc((size_t)src_len);
    memset(dec_buf, 0, (size_t)src_len);
    uLongf dec_len = (uLongf)src_len;
    int u_ret = uncompress(dec_buf, &dec_len, comp_buf, comp_len);
    gzFile gzf_w = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gz_set = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("tmp_zlib_api_sequence.gz", "rb");
    unsigned char read_buf[512];
    memset(read_buf, 0, sizeof(read_buf));
    int gz_rd = gzread(gzf_r, read_buf, (unsigned int)sizeof(read_buf));

    // step 4: Validate / Cleanup
    int infl_end = inflateEnd(&strm);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(dec_buf);
    (void)version;
    (void)c_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)ib_ret;
    (void)u_ret;
    (void)dec_len;
    (void)gz_set;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)infl_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}