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
//<ID> 673
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef dictionary[32];
    memset(dictionary, 'D', (size_t)32);
    uInt dict_len = (uInt)32;
    Bytef plain[256];
    memset(plain, 'A', (size_t)256);
    uLong plain_len = (uLong)256;
    z_stream defstrm;
    memset(&defstrm, 0, (size_t)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = plain;
    defstrm.avail_in = (uInt)plain_len;

    // step 2: Configure
    int def_reset_ret = deflateReset(&defstrm);
    int def_setdict_ret = deflateSetDictionary(&defstrm, dictionary, dict_len);
    uLong def_bound = deflateBound(&defstrm, (uLong)plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)def_bound);
    memset(comp_buf, 0, (size_t)def_bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)def_bound;

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    z_stream infstrm;
    memset(&infstrm, 0, (size_t)sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    Bytef outbuf[256];
    memset(outbuf, 0, (size_t)256);
    infstrm.next_out = outbuf;
    infstrm.avail_out = (uInt)plain_len;
    int inf_setdict_ret = inflateSetDictionary(&infstrm, dictionary, dict_len);
    int inf_ret = inflate(&infstrm, 4);
    uLong inflated_total = infstrm.total_out;
    Bytef gotdict[64];
    memset(gotdict, 0, (size_t)64);
    uInt gotdict_len = (uInt)64;
    int inf_getdict_ret = inflateGetDictionary(&infstrm, gotdict, &gotdict_len);
    int inf_end_ret = inflateEnd(&infstrm);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)def_setdict_ret;
    (void)def_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_setdict_ret;
    (void)inf_ret;
    (void)inflated_total;
    (void)inf_getdict_ret;
    (void)inf_end_ret;
    (void)dictionary;
    (void)dict_len;
    (void)plain;
    (void)plain_len;
    (void)outbuf;
    // API sequence test completed successfully
    return 66;
}