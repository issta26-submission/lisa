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
//<ID> 1644
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateCopy -> deflateReset -> inflate -> gzoffset64/gzeof usage";
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
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int def_copy_ret = deflateCopy(&copy_strm, &def_strm);
    int def_reset_ret = deflateReset(&def_strm);

    // step 3: Operate
    int def_ret = deflate(&copy_strm, 4);
    uLong comp_len = copy_strm.total_out;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, decomp_buf, (unsigned int)(src_len - (uLong)inf_strm.avail_out));
    int gzeof_ret = gzeof(gzf);
    off64_t off64 = gzoffset64(gzf);
    int gz_close_ret = gzclose(gzf);
    int def_end_copy_ret = deflateEnd(&copy_strm);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)comp_bound;
    (void)def_copy_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)gzwrite_ret;
    (void)gzeof_ret;
    (void)off64;
    (void)gz_close_ret;
    (void)def_end_copy_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    return 66;
    // API sequence test completed successfully
}