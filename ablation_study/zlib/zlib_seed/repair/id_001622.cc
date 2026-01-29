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
//<ID> 1622
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: inflateBackInit_, gzflush, inflateReset, uncompress2 usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len_f = comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len_u = (uLong)comp_len_f;
    uLongf dest_len_f = (uLongf)(src_len + 16);
    Bytef *dest_buf = (Bytef *)malloc((size_t)dest_len_f);
    memset(dest_buf, 0, (int)dest_len_f);
    int uncompress_ret = uncompress2(dest_buf, &dest_len_f, comp_buf, &comp_len_u);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int ibinit_ret = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Operate
    int ireset_ret = inflateReset(&strm);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, dest_buf, (unsigned int)dest_len_f);
    int gzflush_ret = gzflush(gzf, 2);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int ibend_ret = inflateBackEnd(&strm);
    free(comp_buf);
    free(dest_buf);
    free(window);
    (void)ver;
    (void)comp_ret;
    (void)uncompress_ret;
    (void)ibinit_ret;
    (void)ireset_ret;
    (void)gw_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)ibend_ret;
    // API sequence test completed successfully
    return 66;
}