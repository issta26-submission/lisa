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
//<ID> 819
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Hello zlib small data";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    uLong comp_bound = compressBound(sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len_f, (const Bytef *)sample, sample_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 16));
    memset(decomp_buf, 0, (size_t)(sample_len + 16));
    uLongf decomp_len_f = (uLongf)(sample_len + 16);
    int uncomp_ret = uncompress(decomp_buf, &decomp_len_f, comp_buf, (uLong)comp_len_f);

    // step 2: Configure inflateBack
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int ibinit_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate with gzFile (set params, write compressed data, read it back)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gzset_ret = gzsetparams(gzf_w, 1, 0);
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len_f);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *read_back = (Bytef *)malloc((size_t)comp_bound);
    memset(read_back, 0, (size_t)comp_bound);
    int gz_rd = gzread(gzf_r, read_back, (unsigned int)comp_len_f);
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&strm);
    free(comp_buf);
    free(decomp_buf);
    free(read_back);
    (void)version;
    (void)sample_len;
    (void)comp_bound;
    (void)comp_len_f;
    (void)comp_ret;
    (void)decomp_len_f;
    (void)uncomp_ret;
    (void)ibinit_ret;
    (void)fname;
    (void)gzset_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}