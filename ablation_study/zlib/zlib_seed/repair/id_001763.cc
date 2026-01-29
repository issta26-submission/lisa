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
//<ID> 1763
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate with dictionary -> gzwrite -> gzread -> inflateInit2_ -> crc32 -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    Bytef dict[] = "example dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int dict_set_ret = deflateSetDictionary(&def_strm, dict, dict_len);

    // step 2: Configure & Deflate
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (write gz, read gz, inflate)
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    gzclearerr(gzf_w);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)comp_len);
    memset(comp_buf2, 0, (int)comp_len);
    int read_bytes = gzread(gzf_r, comp_buf2, (unsigned int)comp_len);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    inf_strm.next_in = comp_buf2;
    inf_strm.avail_in = (uInt)read_bytes;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    uLong crc_val = crc32(0UL, decomp_buf, (uInt)src_len);
    int inf_end_ret = inflateEnd(&inf_strm);
    gzclearerr(gzf_r);
    int gzclose_r = gzclose(gzf_r);
    free(comp_buf);
    free(comp_buf2);
    free(decomp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gzclose_w;
    (void)read_bytes;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)crc_val;
    (void)inf_end_ret;
    (void)gzclose_r;
    // API sequence test completed successfully
    return 66;
}