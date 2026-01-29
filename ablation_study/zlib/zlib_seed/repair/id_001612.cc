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
//<ID> 1612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compressBound -> deflate -> gzclearerr -> zError";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 2: Configure
    (void)def_init_ret;
    (void)ver;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong produced = def_strm.total_out;

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = 0;
    if (gzf) {
        gw_ret = gzwrite(gzf, comp_buf, (unsigned int)produced);
    }
    gzclearerr(gzf);

    const char *err_str = zError(def_ret);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    if (gzf) {
        int gz_close_ret = gzclose(gzf);
        (void)gz_close_ret;
    }
    free(comp_buf);

    (void)gw_ret;
    (void)err_str;
    (void)produced;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}