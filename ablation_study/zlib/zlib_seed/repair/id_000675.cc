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
//<ID> 675
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[512];
    memset(plain_in, 'X', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, plain_in, plain_len);

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = comp_buf;
    def_strm.avail_in = (uInt)comp_len;
    uLong def_out_bound = deflateBound(&def_strm, (uLong)comp_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out, 0, (size_t)def_out_bound);
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)def_out_bound;

    // step 2: Configure
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong def_out_len = def_strm.total_out;
    int def_reset_ret = deflateReset(&def_strm);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = def_out;
    inf_strm.avail_in = (uInt)def_out_len;

    Bytef dict_buf[64];
    memset(dict_buf, 'D', sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int get_dict_ret = inflateGetDictionary(&inf_strm, dict_buf, &dict_len);
    int set_dict_ret = inflateSetDictionary(&inf_strm, dict_buf, dict_len);

    // step 3: Operate
    Bytef decomp[512];
    memset(decomp, 0, sizeof(decomp));
    inf_strm.next_out = decomp;
    inf_strm.avail_out = (uInt)plain_len;
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(def_out);
    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_out_bound;
    (void)def_out_len;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)get_dict_ret;
    (void)set_dict_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)decomp;
    // API sequence test completed successfully
    return 66;
}