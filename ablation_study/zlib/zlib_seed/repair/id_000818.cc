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
//<ID> 818
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[] = "Hello zlib API sequence!";
    uLong plain_len = (uLong)(sizeof(plain) - 1);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);

    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char *window = (unsigned char *)malloc((1 << 15));
    memset(window, 0, (1 << 15));
    int ibi_ret = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef *uncmp_buf = (Bytef *)malloc((size_t)plain_len);
    memset(uncmp_buf, 0, (size_t)plain_len);
    uLongf uncmp_len = plain_len;
    int uncmp_ret = uncompress(uncmp_buf, &uncmp_len, comp_buf, comp_len);

    gzFile gzf_w = gzopen("tmp_zlib_seq.gz", "wb");
    int gz_set_ret = gzsetparams(gzf_w, 6, 0);
    int gz_wr = gzwrite(gzf_w, plain, (unsigned int)plain_len);
    int gz_cl_w = gzclose(gzf_w);

    // step 3: Operate
    gzFile gzf_r = gzopen("tmp_zlib_seq.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)plain_len);
    memset(read_buf, 0, (size_t)plain_len);
    int gz_rd = gzread(gzf_r, read_buf, (unsigned int)plain_len);
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&strm);
    free(comp_buf);
    free(uncmp_buf);
    free(window);
    free(read_buf);
    (void)version;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)ibi_ret;
    (void)uncmp_len;
    (void)uncmp_ret;
    (void)gz_set_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}