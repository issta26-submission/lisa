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
//<ID> 1718
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateParams -> deflate -> gzwrite -> gzoffset64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong adler = adler32(0L, src, (uInt)src_len);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    int def_params_ret = deflateParams(&def_strm, 6, 0);
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_reset_ret = deflateReset(&def_strm);

    // step 4: Validate & Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    off64_t offset64 = gzoffset64(gzf);
    int direct_mode = gzdirect(gzf);
    int gzclose_ret = gzclose(gzf);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)ver;
    (void)adler;
    (void)def_init_ret;
    (void)def_params_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_reset_ret;
    (void)gw_ret;
    (void)offset64;
    (void)direct_mode;
    (void)gzclose_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}