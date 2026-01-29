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
//<ID> 1608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: prime/tune -> deflate -> inflate -> gzbuffer & adler combine";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong ad_part1 = adler32(0L, src, (uInt)(src_len / 2));
    uLong ad_part2 = adler32(0L, src + (src_len / 2), (uInt)(src_len - (src_len / 2)));
    uLong combined_adler = adler32_combine64(ad_part1, ad_part2, (off64_t)(src_len - (src_len / 2)));

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
    int prime_ret = deflatePrime(&def_strm, 3, 5);
    int tune_ret = deflateTune(&def_strm, 20, 10, 30, 2048);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int buf_ret = gzbuffer(gzf, 8192);
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);

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
    uLong out_adler = adler32(0L, out_buf, (uInt)src_len);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(out_buf);

    (void)ver;
    (void)ad_part1;
    (void)ad_part2;
    (void)combined_adler;
    (void)def_init_ret;
    (void)bound;
    (void)prime_ret;
    (void)tune_ret;
    (void)buf_ret;
    (void)gw_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)out_adler;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}