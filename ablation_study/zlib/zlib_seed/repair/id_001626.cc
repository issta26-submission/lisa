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
//<ID> 1626
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
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len_f = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Configure
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int inb_ret = inflateBackInit_(&strm, 15, window, ver, (int)sizeof(z_stream));
    int reset_ret = inflateReset(&strm);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gzflush_ret = gzflush(gzf, 0);

    // step 3: Operate
    Bytef *uncomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(uncomp_buf, 0, (int)src_len);
    uLongf dest_len_f = (uLongf)src_len;
    uLong source_for_uncomp = comp_len;
    int uncomp_ret = uncompress2(uncomp_buf, &dest_len_f, comp_buf, &source_for_uncomp);

    // step 4: Validate & Cleanup
    int inb_end_ret = inflateBackEnd(&strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(uncomp_buf);
    free(window);
    (void)ver;
    (void)comp_ret;
    (void)inb_ret;
    (void)reset_ret;
    (void)gw_ret;
    (void)gzflush_ret;
    (void)uncomp_ret;
    (void)inb_end_ret;
    (void)gzclose_ret;
    (void)dest_len_f;
    (void)source_for_uncomp;
    // API sequence test completed successfully
    return 66;
}