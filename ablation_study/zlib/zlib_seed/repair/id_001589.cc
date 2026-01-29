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
//<ID> 1589
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate with dictionary -> deflateGetDictionary -> inflateGetDictionary -> gzwrite";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong adler = adler32(0L, src, (uInt)src_len);

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
    const Bytef preset_dict[] = "preset_dictionary_data";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    int set_dict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_dict_len);

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gz_close_write_ret = gzclose(gzf);

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
    int inf_set_dict_ret = inflateSetDictionary(&inf_strm, preset_dict, preset_dict_len);
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    Bytef dict_from_inflate[64];
    memset(dict_from_inflate, 0, (int)sizeof(dict_from_inflate));
    uInt dict_from_inflate_len = (uInt)sizeof(dict_from_inflate);
    int inf_get_dict_ret = inflateGetDictionary(&inf_strm, dict_from_inflate, &dict_from_inflate_len);

    Bytef dict_from_deflate[64];
    memset(dict_from_deflate, 0, (int)sizeof(dict_from_deflate));
    uInt dict_from_deflate_len = (uInt)sizeof(dict_from_deflate);
    int def_get_dict_ret = deflateGetDictionary(&def_strm, dict_from_deflate, &dict_from_deflate_len);

    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);

    free(comp_buf);
    free(out_buf);

    (void)ver;
    (void)adler;
    (void)def_init_ret;
    (void)def_bound;
    (void)set_dict_ret;
    (void)gw_ret;
    (void)gz_close_write_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_set_dict_ret;
    (void)inf_ret;
    (void)inf_get_dict_ret;
    (void)def_get_dict_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}