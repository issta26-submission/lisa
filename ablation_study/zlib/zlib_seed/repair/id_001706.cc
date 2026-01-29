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
//<ID> 1706
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: inflateInit_ -> compress2 -> uncompress -> gzputs -> gzgetc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, (uLong)src_len, 6);

    // step 2: Configure
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(decomp_buf, 0, (int)(src_len + 1));
    uLongf decomp_len = src_len;
    int uncmp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, (uLong)comp_len);
    decomp_buf[decomp_len] = 0; // ensure null-terminated for gzputs
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 3: Operate
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gp_ret = gzputs(gzf_w, (const char *)decomp_buf);
    int gzw_close = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gzf_r);
    int second_char = gzgetc(gzf_r);

    // step 4: Validate & Cleanup
    int gzr_close = gzclose(gzf_r);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)comp_ret;
    (void)uncmp_ret;
    (void)inf_init_ret;
    (void)gp_ret;
    (void)gzw_close;
    (void)first_char;
    (void)second_char;
    (void)gzr_close;
    (void)inf_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}