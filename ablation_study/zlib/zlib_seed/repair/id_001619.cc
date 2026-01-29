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
//<ID> 1619
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compressBound + gzclearerr + zError + deflate usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    gzclearerr(gzf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 4: Validate & Cleanup
    const char *err_msg = zError((int)def_ret);
    int def_end_ret = deflateEnd(&def_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);

    (void)ver;
    (void)def_init_ret;
    (void)comp_bound;
    (void)gw_ret;
    (void)comp_len;
    (void)err_msg;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}