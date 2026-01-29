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
//<ID> 676
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
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef dict[32];
    memset(dict, 'D', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);

    // step 2: Configure
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&defstrm);
    defstrm.next_in = plain_in;
    defstrm.avail_in = (uInt)plain_len;
    uLong comp2_bound = deflateBound(&defstrm, plain_len);
    Bytef *comp2_buf = (Bytef *)malloc((size_t)comp2_bound);
    memset(comp2_buf, 0, (size_t)comp2_bound);
    defstrm.next_out = comp2_buf;
    defstrm.avail_out = (uInt)comp2_bound;

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp2_len = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = comp2_buf;
    infstrm.avail_in = (uInt)comp2_len;
    Bytef decomp[256];
    memset(decomp, 0, sizeof(decomp));
    infstrm.next_out = decomp;
    infstrm.avail_out = (uInt)plain_len;
    int inf_setdict_ret = inflateSetDictionary(&infstrm, dict, dict_len);
    int inf_ret = inflate(&infstrm, 4);
    Bytef dict_out[64];
    memset(dict_out, 0, sizeof(dict_out));
    uInt got_len = (uInt)sizeof(dict_out);
    int inf_getdict_ret = inflateGetDictionary(&infstrm, dict_out, &got_len);
    int inf_end_ret = inflateEnd(&infstrm);

    // step 4: Validate / Cleanup
    free(comp2_buf);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)comp2_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_setdict_ret;
    (void)inf_ret;
    (void)inf_getdict_ret;
    (void)inf_end_ret;
    (void)dict_out;
    // API sequence test completed successfully
    return 66;
}