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
//<ID> 1601
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: use deflateTune, deflatePrime, gzbuffer, and adler32_combine64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong ad1 = adler32(0L, src, (uInt)(src_len / 2));
    uLong ad2 = adler32(0L, src + (src_len / 2), (uInt)(src_len - (src_len / 2)));
    off64_t part2_len = (off64_t)(src_len - (src_len / 2));
    uLong combined_adler = adler32_combine64(ad1, ad2, part2_len);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Configure
    int tune_ret = deflateTune(&def_strm, 20, 10, 30, 2048);
    int prime_ret = deflatePrime(&def_strm, 3, 5);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_set = gzbuffer(gzf, 16384);
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);

    (void)ver;
    (void)ad1;
    (void)ad2;
    (void)combined_adler;
    (void)def_init_ret;
    (void)bound;
    (void)tune_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)buf_set;
    (void)gw_ret;
    (void)gz_close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}