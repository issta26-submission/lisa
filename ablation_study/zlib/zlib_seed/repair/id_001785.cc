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
//<ID> 1785
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: dictionary-based deflate -> inflate sequence";
    const Bytef dict[] = "example_dictionary_data_used_for_both_sides";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    (void)def_init_ret;

    // step 2: Configure & Compress (use dictionary on deflate side)
    int setdict_def_ret = deflateSetDictionary(&def_strm, dict, dict_len);
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
    (void)setdict_def_ret;
    (void)def_ret;
    (void)def_end_ret;

    // step 3: Prepare inflate, set dictionary, and operate
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (int)(src_len + 64));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_setdict_ret = inflateSetDictionary(&inf_strm, dict, dict_len);
    int inf_ret = inflate(&inf_strm, 0);
    (void)inf_init_ret;
    (void)inf_setdict_ret;
    (void)inf_ret;

    // step 4: Validate & Cleanup
    int inf_back_end_ret = inflateBackEnd(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)bound;
    (void)comp_len;
    (void)inf_back_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}