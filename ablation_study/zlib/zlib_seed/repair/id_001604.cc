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
//<ID> 1604
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef part1[] = "Hello ";
    const Bytef part2[] = "zlib API";
    uLong len1 = (uLong)(sizeof(part1) - 1);
    uLong len2 = (uLong)(sizeof(part2) - 1);
    uLong total_len = len1 + len2;
    const char *ver = zlibVersion();

    uLong ad1 = adler32(0L, part1, (uInt)len1);
    uLong ad2 = adler32(0L, part2, (uInt)len2);
    uLong combined_adler = adler32_combine64(ad1, ad2, (off64_t)len2);

    Bytef *src = (Bytef *)malloc((size_t)total_len);
    memcpy(src, part1, (size_t)len1);
    memcpy(src + len1, part2, (size_t)len2);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = src;
    def_strm.avail_in = (uInt)total_len;

    uLong bound = deflateBound(&def_strm, total_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Configure
    int tune_ret = deflateTune(&def_strm, 16, 8, 24, 1024);
    int prime_ret = deflatePrime(&def_strm, 3, 5);

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gzf, 8192);
    int gw_ret = gzwrite(gzf, src, (unsigned int)total_len);
    int gz_close_write_ret = gzclose(gzf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)total_len);
    memset(out_buf, 0, (int)total_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)total_len;
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    uLong ad_after = adler32(0L, out_buf, (uInt)total_len);

    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);

    free(src);
    free(comp_buf);
    free(out_buf);

    (void)ver;
    (void)ad1;
    (void)ad2;
    (void)combined_adler;
    (void)def_init_ret;
    (void)bound;
    (void)tune_ret;
    (void)prime_ret;
    (void)buf_ret;
    (void)gw_ret;
    (void)gz_close_write_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)ad_after;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}