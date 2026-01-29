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
//<ID> 1580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: adler32 -> deflate with dictionary -> deflateGetDictionary -> inflateGetDictionary -> gzwrite";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong ad_before = adler32(0L, src, (uInt)src_len);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *def_out = (Bytef *)malloc((size_t)bound);
    memset(def_out, 0, (int)bound);
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)bound;

    // step 2: Configure
    const Bytef dict[] = "preset_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int set_dict_ret = deflateSetDictionary(&def_strm, dict, dict_len);
    uInt def_get_len = dict_len;
    Bytef *def_get_buf = (Bytef *)malloc((size_t)def_get_len);
    memset(def_get_buf, 0, (int)def_get_len);
    int def_get_ret = deflateGetDictionary(&def_strm, def_get_buf, &def_get_len);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 0);
    uLong comp_len = def_strm.total_out;
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzw, def_out, (unsigned int)comp_len);
    int gw_close_ret = gzclose(gzw);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = def_out;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_set_dict_ret = inflateSetDictionary(&inf_strm, dict, dict_len);
    uInt inf_get_len = dict_len;
    Bytef *inf_get_buf = (Bytef *)malloc((size_t)inf_get_len);
    memset(inf_get_buf, 0, (int)inf_get_len);
    int inf_get_ret = inflateGetDictionary(&inf_strm, inf_get_buf, &inf_get_len);
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    uLong ad_after = adler32(0L, out_buf, (uInt)src_len);
    uLong ad_combo = adler32(ad_before, def_get_buf, def_get_len);
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(def_out);
    free(def_get_buf);
    free(out_buf);
    free(inf_get_buf);
    (void)ver;
    (void)def_init_ret;
    (void)set_dict_ret;
    (void)def_get_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gzw;
    (void)gw_ret;
    (void)gw_close_ret;
    (void)inf_init_ret;
    (void)inf_set_dict_ret;
    (void)inf_get_ret;
    (void)inf_ret;
    (void)ad_before;
    (void)ad_after;
    (void)ad_combo;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}