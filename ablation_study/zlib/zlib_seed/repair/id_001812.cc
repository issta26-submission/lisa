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
//<ID> 1812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit2_ -> deflate -> inflate -> gz write/read -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *ver = zlibVersion();
    int def_init2_ret = deflateInit2_(&def_strm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (inflate compressed buffer) & gz write/read
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 128));
    memset(decomp_buf, 0, (int)(src_len + 128));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 128);
    int inf_ret = inflate(&inf_strm, 4);
    int sync_point_ret = inflateSyncPoint(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gzrewind_ret = gzrewind(gzf_r);
    Bytef *gz_read_buf = (Bytef *)malloc((size_t)(src_len + 128));
    memset(gz_read_buf, 0, (int)(src_len + 128));
    int gzread_ret = gzread(gzf_r, gz_read_buf, (unsigned int)(src_len + 128));
    int gzclose_r_ret = gzclose_r(gzf_r);

    // step 4: Validate & Cleanup
    uLong check_adler_inflated = adler32(0UL, decomp_buf, (uInt)(src_len));
    uLong check_adler_gz = adler32(0UL, gz_read_buf, (uInt)(gzread_ret > 0 ? (uInt)gzread_ret : 0U));
    free(comp_buf);
    free(decomp_buf);
    free(gz_read_buf);
    (void)def_init2_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)sync_point_ret;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzrewind_ret;
    (void)gzread_ret;
    (void)gzclose_r_ret;
    (void)check_adler_inflated;
    (void)check_adler_gz;
    // API sequence test completed successfully
    return 66;
}