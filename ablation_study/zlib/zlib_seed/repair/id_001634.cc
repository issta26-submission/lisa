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
//<ID> 1634
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> deflatePending -> inflate -> gzopen64 usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending, &bits);
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Configure
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)src_len;

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 4);
    uLong decomp_len = inf_strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)decomp_len);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)pending;
    (void)bits;
    (void)pending_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)gw_ret;
    (void)gz_close_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}