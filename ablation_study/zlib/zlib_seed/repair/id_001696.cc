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
//<ID> 1696
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence: deflate -> inflate -> write gz -> read gz with gzgetc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);
    int inf_reset_keep_ret = inflateResetKeep(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate & Cleanup
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzw, decomp_buf, (unsigned int)(src_len - (uLong)inf_strm.avail_out));
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc(gzr);
    int gzclose_r_ret = gzclose(gzr);
    free(comp_buf);
    free(decomp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)bound;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_reset_keep_ret;
    (void)inf_end_ret;
    (void)gw_ret;
    (void)gzclose_w_ret;
    (void)first_char;
    (void)gzclose_r_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}