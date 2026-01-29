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
//<ID> 1628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: inflateBackInit_ + gzflush + inflateReset + uncompress2 usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (int)(1 << 15));
    const char *ver = zlibVersion();
    int ibi_ret = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gzflush_ret = gzflush(gzf, 2);

    // step 3: Operate
    uLongf dest_len_f = (uLongf)src_len;
    Bytef *dest_buf = (Bytef *)malloc((size_t)dest_len_f);
    memset(dest_buf, 0, (int)dest_len_f);
    uLong src_len_for_uncompress = comp_len;
    int uncmp_ret = uncompress2(dest_buf, &dest_len_f, comp_buf, &src_len_for_uncompress);
    int ir_ret = inflateReset(&strm);

    // step 4: Validate & Cleanup
    int ibe_ret = inflateBackEnd(&strm);
    int gz_close_ret = gzclose(gzf);
    free(window);
    free(comp_buf);
    free(dest_buf);

    (void)comp_ret;
    (void)comp_len_f;
    (void)ibi_ret;
    (void)gw_ret;
    (void)gzflush_ret;
    (void)uncmp_ret;
    (void)ir_ret;
    (void)ibe_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}