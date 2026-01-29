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
//<ID> 1702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Sample zlib data for API sequence test";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong dest_buf_size = src_len + 16;
    Bytef *dest_buf = (Bytef *)malloc((size_t)dest_buf_size);
    memset(dest_buf, 0, (int)dest_buf_size);
    uLongf dest_len = (uLongf)dest_buf_size;
    int unret = uncompress(dest_buf, &dest_len, comp_buf, comp_len);

    // step 3: Operate
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzputs(gzf_w, (const char *)dest_buf);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gzf_r);
    int second_char = gzgetc(gzf_r);
    int gzclose_r = gzclose(gzf_r);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(dest_buf);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)unret;
    (void)gw_ret;
    (void)gzclose_w;
    (void)first_char;
    (void)second_char;
    (void)gzclose_r;
    (void)inf_end_ret;
    (void)ver;
    (void)bound;
    (void)dest_len;
    (void)comp_len;
    (void)src_len;
    // API sequence test completed successfully
    return 66;
}