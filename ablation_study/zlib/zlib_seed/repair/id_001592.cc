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
//<ID> 1592
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateTune -> deflateGetDictionary -> gztell64 -> inflateGetHeader";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)def_bound);
    memset(comp_buf, 0, (int)def_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)def_bound;

    const Bytef preset_dict[] = "preset_dictionary_for_test";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    int set_dict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_dict_len);
    int tune_ret = deflateTune(&def_strm, 4, 4, 32, 128);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    off64_t gz_pos = gztell64(gzf);
    int gz_close_ret = gzclose(gzf);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (int)sizeof(header));
    int inf_get_header_ret = inflateGetHeader(&inf_strm, &header);

    // step 4: Validate & Cleanup
    Bytef dict_out[128];
    memset(dict_out, 0, (int)sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int def_get_dict_ret = deflateGetDictionary(&def_strm, dict_out, &dict_out_len);

    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    free(comp_buf);

    (void)ver;
    (void)def_init_ret;
    (void)def_bound;
    (void)set_dict_ret;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gzf;
    (void)gw_ret;
    (void)gz_pos;
    (void)gz_close_ret;
    (void)inf_init_ret;
    (void)inf_get_header_ret;
    (void)def_get_dict_ret;
    (void)def_end_ret;
    (void)inf_end_ret;

    // API sequence test completed successfully
    return 66;
}