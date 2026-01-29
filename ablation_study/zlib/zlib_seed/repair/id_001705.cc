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
//<ID> 1705
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
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));

    // step 2: Configure (compress)
    uLongf comp_alloc = (uLongf)compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_alloc);
    memset(comp_buf, 0, (int)comp_alloc);
    uLongf comp_len = comp_alloc;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 3: Operate (uncompress)
    uLongf decomp_alloc = (uLongf)(src_len + 1);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_alloc);
    memset(decomp_buf, 0, (int)decomp_alloc);
    uLongf decomp_len = decomp_alloc;
    int uncmp_ret = uncompress(decomp_buf, &decomp_len, comp_buf, comp_len);
    decomp_buf[(size_t)(decomp_len < decomp_alloc ? decomp_len : decomp_alloc - 1)] = 0;

    // step 4: Validate & Cleanup
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gputs_ret = gzputs(gzf_w, (const char *)decomp_buf);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gzf_r);
    int gzclose_r = gzclose(gzf_r);
    int inf_end_ret = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    (void)inf_init_ret;
    (void)comp_ret;
    (void)uncmp_ret;
    (void)gputs_ret;
    (void)gzclose_w;
    (void)first_char;
    (void)gzclose_r;
    (void)inf_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}