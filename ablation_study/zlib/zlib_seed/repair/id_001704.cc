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
//<ID> 1704
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: zlibVersion -> compress2 -> uncompress -> gzputs -> gzgetc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len_f = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);

    // step 2: Configure
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    uLongf decomp_len = (uLongf)src_len;
    int uncompress_ret = uncompress(decomp_buf, &decomp_len, comp_buf, (uLong)comp_len_f);

    // step 3: Operate
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzputs(gzf_w, (const char *)decomp_buf);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gzf_r);
    int second_char = gzgetc(gzf_r);

    // step 4: Validate & Cleanup
    int gzclose_r = gzclose(gzf_r);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)inf_init_ret;
    (void)bound;
    (void)comp_ret;
    (void)comp_len_f;
    (void)uncompress_ret;
    (void)decomp_len;
    (void)gw_ret;
    (void)gzclose_w;
    (void)first_char;
    (void)second_char;
    (void)gzclose_r;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}