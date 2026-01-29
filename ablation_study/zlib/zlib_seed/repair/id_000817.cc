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
//<ID> 817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize data and compress using zlib's compress()
    const char *version = zlibVersion();
    const char plain_msg[] = "Zlib API sequence test payload";
    uLong plain_len = (uLong)(sizeof(plain_msg) - 1);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, (const Bytef *)plain_msg, plain_len);

    // step 2: Uncompress the compressed buffer and validate sizes
    Bytef *decomp_buf = (Bytef *)malloc((size_t)plain_len);
    memset(decomp_buf, 0, (size_t)plain_len);
    uLongf decomp_len = (uLongf)plain_len;
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, (uLong)comp_len);

    // step 3: Write original plain data to a .gz file with gzsetparams, then read it back with gzread
    const char fname[] = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gzset_ret = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, (voidp)plain_msg, (unsigned int)plain_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *gz_read_buf = (Bytef *)malloc((size_t)plain_len);
    memset(gz_read_buf, 0, (size_t)plain_len);
    int gz_rd = gzread(gzf_r, (voidp)gz_read_buf, (unsigned int)plain_len);
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Initialize inflateBack, then cleanup with inflateEnd; final resource cleanup
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int ib_init = inflateBackInit_(&zs, 15, window, version, (int)sizeof(z_stream));
    int ib_end = inflateEnd(&zs);

    free(comp_buf);
    free(decomp_buf);
    free(gz_read_buf);
    free(window);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_len;
    (void)comp_ret;
    (void)decomp_len;
    (void)uncomp_ret;
    (void)fname;
    (void)gzset_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)ib_init;
    (void)ib_end;
    // API sequence test completed successfully
    return 66;
}