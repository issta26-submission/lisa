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
//<ID> 672
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef dict[32];
    memset(dict, 'D', sizeof(dict));
    Bytef plain[128];
    memset(plain, 'P', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    // step 2: Configure
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&defstrm, dict, (uInt)sizeof(dict));
    defstrm.next_in = plain;
    defstrm.avail_in = (uInt)plain_len;
    uLong comp_bound = deflateBound(&defstrm, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    int def_reset_ret = deflateReset(&defstrm);

    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    Bytef decomp[128];
    memset(decomp, 0, sizeof(decomp));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    infstrm.next_out = decomp;
    infstrm.avail_out = (uInt)plain_len;
    int inf_ret1 = inflate(&infstrm, 4);
    int inf_setdict_ret = inflateSetDictionary(&infstrm, dict, (uInt)sizeof(dict));
    Bytef dict_copy[32];
    memset(dict_copy, 0, sizeof(dict_copy));
    uInt dict_copy_len = (uInt)sizeof(dict_copy);
    int inf_getdict_ret = inflateGetDictionary(&infstrm, dict_copy, &dict_copy_len);
    int inf_ret2 = inflate(&infstrm, 4);

    // step 4: Validate / Cleanup
    uLong checksum = adler32(1L, decomp, (uInt)plain_len);
    int inf_end_ret = inflateEnd(&infstrm);
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf);

    (void)version;
    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)inf_ret1;
    (void)inf_setdict_ret;
    (void)inf_getdict_ret;
    (void)inf_ret2;
    (void)checksum;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)dict_copy_len;
    (void)plain_len;
    (void)plain;
    (void)decomp;
    // API sequence test completed successfully
    return 66;
}