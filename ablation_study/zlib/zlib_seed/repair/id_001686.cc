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
//<ID> 1686
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence: deflateBound -> write gz -> reopen gz -> adler32_combine -> gzungetc";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_ret_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int gu_ret = gzungetc(first_char, gzf_r);
    int second_char = gzgetc_(gzf_r);
    int gzclose_ret_r = gzclose(gzf_r);

    // step 4: Validate & Cleanup
    uLong ad1 = adler32(0UL, src, (uInt)src_len);
    uLong ad2 = adler32(0UL, comp_buf, (uInt)comp_len);
    uLong ad_comb = adler32_combine(ad1, ad2, (off_t)comp_len);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_ret_w;
    (void)first_char;
    (void)gu_ret;
    (void)second_char;
    (void)gzclose_ret_r;
    (void)ad1;
    (void)ad2;
    (void)ad_comb;
    (void)def_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}