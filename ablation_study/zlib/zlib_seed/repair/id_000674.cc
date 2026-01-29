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
//<ID> 674
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
    memset(plain_in, 'D', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef dict[256];
    memset(dict, 'X', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);

    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    defstrm.next_in = plain_in;
    defstrm.avail_in = (uInt)plain_len;
    uLong comp_bound = deflateBound(&defstrm, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)comp_bound;
    int dict_set_ret = deflateSetDictionary(&defstrm, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4); /* 4 == Z_FINISH */
    uLong comp_len = defstrm.total_out;
    int def_reset_ret = deflateReset(&defstrm);

    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    infstrm.next_out = decomp_buf;
    infstrm.avail_out = (uInt)plain_len;
    int inf_set_dict_ret = inflateSetDictionary(&infstrm, dict, dict_len);
    int inf_ret = inflate(&infstrm, 4); /* 4 == Z_FINISH */

    // step 4: Validate / Cleanup
    Bytef got_dict[256];
    memset(got_dict, 0, sizeof(got_dict));
    uInt got_dict_len = (uInt)sizeof(got_dict);
    int get_dict_ret = inflateGetDictionary(&infstrm, got_dict, &got_dict_len);
    int inf_end_ret = inflateEnd(&infstrm);
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf);
    (void)version;
    (void)plain_len;
    (void)dict_len;
    (void)def_init_ret;
    (void)comp_bound;
    (void)dict_set_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)inf_set_dict_ret;
    (void)inf_ret;
    (void)got_dict_len;
    (void)get_dict_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)decomp_buf;
    // API sequence test completed successfully
    return 66;
}