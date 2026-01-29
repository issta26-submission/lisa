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
//<ID> 1606
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateTune + deflatePrime + gzbuffer + adler32_combine64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    uLong adler_a = adler32(0L, src, (uInt)src_len);
    const Bytef extra[] = "extra data block";
    uLong extra_len = (uLong)(sizeof(extra) - 1);
    uLong adler_b = adler32(0L, extra, (uInt)extra_len);
    uLong combined_adler = adler32_combine64(adler_a, adler_b, (off64_t)extra_len);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)def_bound);
    memset(comp_buf, 0, (int)def_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)def_bound;

    // step 2: Configure
    int tune_ret = deflateTune(&def_strm, 20, 10, 30, 2048);
    int prime_ret = deflatePrime(&def_strm, 3, 5);

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gzf, 8192);
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gz_close_ret = gzclose(gzf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    uLong adler_out = adler32(0L, out_buf, (uInt)src_len);
    uLong combined2 = adler32_combine64(adler_out, adler_a, (off64_t)1);

    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);

    free(comp_buf);
    free(out_buf);

    (void)ver;
    (void)adler_a;
    (void)adler_b;
    (void)combined_adler;
    (void)def_init_ret;
    (void)def_bound;
    (void)tune_ret;
    (void)prime_ret;
    (void)gzf;
    (void)buf_ret;
    (void)gw_ret;
    (void)gz_close_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)adler_out;
    (void)combined2;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}