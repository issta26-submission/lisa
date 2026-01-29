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
//<ID> 1653
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: inflateInit2_ -> inflate -> deflateSetDictionary -> gzopen usage";
    const Bytef dict[] = "preset_dictionary";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    const char *ver = zlibVersion();

    uLong comp_bound = compressBound(src_len);
    uLongf comp_len = (uLongf)comp_bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);

    // step 2: Configure
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));

    z_stream def_strm2;
    memset(&def_strm2, 0, (int)sizeof(def_strm2));
    int def2_init_ret = deflateInit2_(&def_strm2, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    int dict_set_ret = deflateSetDictionary(&def_strm2, dict, dict_len);

    // step 3: Operate
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (int)src_len);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int wrote = 0;
    wrote = (unsigned int)(src_len - (uLong)inf_strm.avail_out);
    int gw_ret = gzwrite(gzf, decomp_buf, wrote);

    uLong adler = adler32(0L, decomp_buf, (uInt)wrote);

    // step 4: Validate & Cleanup
    int def2_end_ret = deflateEnd(&def_strm2);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)comp_ret;
    (void)comp_bound;
    (void)inf_init_ret;
    (void)def2_init_ret;
    (void)dict_set_ret;
    (void)inf_ret;
    (void)gw_ret;
    (void)adler;
    (void)def2_end_ret;
    (void)inf_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}