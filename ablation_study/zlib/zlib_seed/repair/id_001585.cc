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
//<ID> 1585
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
    uLong ad = adler32(0L, src, (uInt)src_len);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    const Bytef preset_dict[] = "preset_dictionary_for_deflate";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    int set_dict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_dict_len);
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Configure
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &def_strm);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    Bytef dict_out[64];
    memset(dict_out, 0, (int)sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int get_def_dict_ret = deflateGetDictionary(&def_strm, dict_out, &dict_out_len);

    // step 3: Operate
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int get_inf_dict_ret = inflateGetDictionary(&inf_strm, dict_out, &dict_out_len);
    int inf_ret = inflate(&inf_strm, 0);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int inf_end = inflateEnd(&inf_strm);
    int def_end = deflateEnd(&def_strm);
    int copy_end = deflateEnd(&copy_strm);
    free(comp_buf);
    free(out_buf);
    (void)ad;
    (void)ver;
    (void)def_init_ret;
    (void)set_dict_ret;
    (void)bound;
    (void)copy_ret;
    (void)def_ret;
    (void)comp_len;
    (void)get_def_dict_ret;
    (void)inf_init_ret;
    (void)get_inf_dict_ret;
    (void)inf_ret;
    (void)gw_ret;
    (void)gz_close_ret;
    (void)inf_end;
    (void)def_end;
    (void)copy_end;
    // API sequence test completed successfully
    return 66;
}