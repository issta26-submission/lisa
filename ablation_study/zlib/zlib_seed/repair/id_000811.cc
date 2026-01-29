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
//<ID> 811
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char src[] = "Hello zlib API sequence!";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong comp_bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, (const Bytef *)src, src_len, 6);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    unsigned char * window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    const char * version = zlibVersion();
    int ib_init = inflateBackInit_(&strm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    const char * fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gz_w = gzopen(fname, "wb");
    int setp = gzsetparams(gz_w, 6, 0);
    int gz_wr = gzwrite(gz_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gz_w);
    gzFile gz_r = gzopen(fname, "rb");
    Bytef * read_buf = (Bytef *)malloc((size_t)(comp_len + 10));
    memset(read_buf, 0, (size_t)(comp_len + 10));
    int gz_rd = gzread(gz_r, (voidp)read_buf, (unsigned int)(comp_len + 10));
    int gz_cl_r = gzclose(gz_r);

    // step 4: Validate / Cleanup
    uLongf decomp_len = src_len + 10;
    Bytef * decomp_buf = (Bytef *)malloc((size_t)decomp_len);
    memset(decomp_buf, 0, (size_t)decomp_len);
    int uncomp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    int inf_end = inflateEnd(&strm);
    free(comp_buf);
    free(window);
    free(read_buf);
    free(decomp_buf);
    (void)comp_ret;
    (void)ib_init;
    (void)setp;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)uncomp_ret;
    (void)inf_end;
    (void)fname;
    (void)version;
    (void)src_len;
    (void)comp_bound;
    (void)comp_len;
    (void)decomp_len;
    // API sequence test completed successfully
    return 66;
}