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
//<ID> 1605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: use deflatePrime, deflateTune, gzbuffer, adler32_combine64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    uLong half_len = (uLong)(src_len / 2U);
    uLong adler_first = adler32(0L, src, (uInt)half_len);
    uLong adler_second = adler32(0L, src + half_len, (uInt)(src_len - half_len));
    uLong combined_adler = adler32_combine64(adler_first, adler_second, (off64_t)(src_len - half_len));

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)def_bound);
    memset(comp_buf, 0, (int)def_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)def_bound;

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);

    // step 2: Configure
    int buf_ret = gzbuffer(gzf, 8192);
    int prime_ret = deflatePrime(&def_strm, 3, 5);
    int tune_ret = deflateTune(&def_strm, 20, 10, 30, 2048);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);

    (void)ver;
    (void)init_ret;
    (void)def_bound;
    (void)gw_ret;
    (void)buf_ret;
    (void)prime_ret;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gz_close_ret;
    (void)adler_first;
    (void)adler_second;
    (void)combined_adler;
    // API sequence test completed successfully
    return 66;
}