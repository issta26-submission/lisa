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
//<ID> 671
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain_in, plain_len, 6);

    // step 2: Configure
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = comp_buf;
    defstrm.avail_in = (uInt)comp_len;
    Bytef *def_out = (Bytef *)malloc((size_t)comp_bound);
    memset(def_out, 0, (size_t)comp_bound);
    defstrm.next_out = def_out;
    defstrm.avail_out = (uInt)comp_bound;
    int def_reset_ret = deflateReset(&defstrm);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4); /* 4 == Z_FINISH */
    uLong def_out_len = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = def_out;
    infstrm.avail_in = (uInt)def_out_len;
    Bytef decomp[256];
    memset(decomp, 0, sizeof(decomp));
    infstrm.next_out = decomp;
    infstrm.avail_out = (uInt)plain_len;
    Bytef dict[16];
    memset(dict, 'D', sizeof(dict));
    int inf_setdic_ret = inflateSetDictionary(&infstrm, dict, (uInt)sizeof(dict));
    int inf_ret = inflate(&infstrm, 4); /* 4 == Z_FINISH */
    Bytef out_dict[16];
    memset(out_dict, 0, sizeof(out_dict));
    uInt out_dict_len = (uInt)sizeof(out_dict);
    int inf_getdic_ret = inflateGetDictionary(&infstrm, out_dict, &out_dict_len);
    int inf_end_ret = inflateEnd(&infstrm);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(def_out);
    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)def_out_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_setdic_ret;
    (void)inf_ret;
    (void)inf_getdic_ret;
    (void)inf_end_ret;
    (void)decomp;
    (void)dict;
    (void)out_dict;
    // API sequence test completed successfully
    return 66;
}