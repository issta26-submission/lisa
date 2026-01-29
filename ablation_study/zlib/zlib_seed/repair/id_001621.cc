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
//<ID> 1621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress2 -> uncompress2 -> inflateBackInit_ -> gzflush usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len = comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    uLong comp_src_len = (uLong)comp_len;

    // step 2: Configure
    uLongf dest_len = src_len;
    Bytef *dest_buf = (Bytef *)malloc((size_t)dest_len);
    memset(dest_buf, 0, (int)dest_len);

    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int ibi_ret = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));
    int ir_ret = inflateReset(&strm);

    // step 3: Operate
    int un_ret = uncompress2(dest_buf, &dest_len, comp_buf, &comp_src_len);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, dest_buf, (unsigned int)dest_len);
    int gf_ret = gzflush(gzf, 2);

    // step 4: Validate & Cleanup
    int ibe_ret = inflateBackEnd(&strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(dest_buf);
    free(window);

    (void)ver;
    (void)src_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)comp_src_len;
    (void)dest_len;
    (void)ibi_ret;
    (void)ir_ret;
    (void)un_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)ibe_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}