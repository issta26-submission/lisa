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
//<ID> 816
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const char plain[] = "Zlib sample data for API sequence";
    uLong plain_len = (uLong)(sizeof(plain) - 1);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, (const Bytef *)plain, plain_len, 6);

    z_stream zinf;
    memset(&zinf, 0, sizeof(zinf));
    int inf_init_ret = inflateInit_(&zinf, version, (int)sizeof(z_stream));

    // step 2: Configure
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gzset_ret = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, plain, (unsigned int)plain_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)plain_len);
    memset(read_buf, 0, (size_t)plain_len);
    int gz_rd = gzread(gzf_r, read_buf, (unsigned int)plain_len);

    // step 3: Operate
    Bytef *decomp_buf = (Bytef *)malloc((size_t)plain_len);
    memset(decomp_buf, 0, (size_t)plain_len);
    uLongf decomp_len = (uLongf)plain_len;
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);

    z_stream zback;
    memset(&zback, 0, sizeof(zback));
    unsigned char *window = (unsigned char *)malloc(1 << 15);
    memset(window, 0, (1 << 15));
    int back_init = inflateBackInit_(&zback, 15, window, version, (int)sizeof(z_stream));
    int back_end = inflateBackEnd(&zback);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&zinf);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    free(window);
    (void)version;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)comp_len;
    (void)gzset_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)uncomp_ret;
    (void)back_init;
    (void)back_end;
    (void)inf_end;
    (void)gz_cl_r;
    (void)fname;
    (void)decomp_len;
    // API sequence test completed successfully
    return 66;
}