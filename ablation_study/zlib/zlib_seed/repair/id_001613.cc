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
//<ID> 1613
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compressBound + deflate + gzwrite + gzclearerr + zError";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong comp_cap = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_cap);
    memset(comp_buf, 0, (int)comp_cap);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_cap;

    // step 2: Configure
    int tune_ret = deflateTune(&def_strm, 20, 10, 30, 2048);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    gzclearerr(gzf);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    const char *err_str = zError(def_end_ret);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)comp_cap;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_ret;
    (void)gz_close_ret;
    (void)err_str;
    // API sequence test completed successfully
    return 66;
}