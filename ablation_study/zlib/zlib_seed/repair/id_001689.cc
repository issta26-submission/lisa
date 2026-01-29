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
//<ID> 1689
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateBound -> deflate -> adler32_combine -> gzungetc -> gzclose";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    uInt len1 = (uInt)(src_len / 2);
    uInt len2 = (uInt)(src_len - len1);
    uLong ad1 = adler32(0L, src, len1);
    uLong ad2 = adler32(0L, src + len1, len2);
    uLong combined_adler = adler32_combine(ad1, ad2, (off_t)len2);

    // step 4: Validate & Cleanup
    gzFile gzf_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gzclose_ret_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence.gz", "rb");
    int first_char = gzgetc(gzf_r);
    int ungot_ret = gzungetc(first_char, gzf_r);
    int second_char = gzgetc(gzf_r);
    int gzclose_ret_r = gzclose(gzf_r);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)bound;
    (void)ad1;
    (void)ad2;
    (void)combined_adler;
    (void)gw_ret;
    (void)gzclose_ret_w;
    (void)first_char;
    (void)ungot_ret;
    (void)second_char;
    (void)gzclose_ret_r;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}