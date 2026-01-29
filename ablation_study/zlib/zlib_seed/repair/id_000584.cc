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
//<ID> 584
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[128];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    Bytef preset_dict[32];
    memset(preset_dict, 0x5A, sizeof(preset_dict));
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&defstrm, preset_dict, (uInt)sizeof(preset_dict));
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef compbuf[512];
    memset(compbuf, 0, sizeof(compbuf));
    defstrm.next_in = plain;
    defstrm.avail_in = (uInt)plain_len;
    defstrm.next_out = compbuf;
    defstrm.avail_out = (uInt)sizeof(compbuf);
    uLong bound_est = deflateBound(&defstrm, plain_len);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4); /* 4 == Z_FINISH */
    uLong comp_len = defstrm.total_out;
    uLong checksum = crc32(0L, plain, (uInt)plain_len);
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write_ret = gzwrite(gw, compbuf, (unsigned int)comp_len);

    // step 4: Validate / Cleanup
    infstrm.next_in = compbuf;
    infstrm.avail_in = (uInt)comp_len;
    Bytef outbuf[256];
    memset(outbuf, 0, sizeof(outbuf));
    infstrm.next_out = outbuf;
    infstrm.avail_out = (uInt)sizeof(outbuf);
    int inf_ret = inflate(&infstrm, 4); /* 4 == Z_FINISH */
    Bytef dict_out[64];
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int inf_getdict_ret = inflateGetDictionary(&infstrm, dict_out, &dict_out_len);
    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&infstrm);
    int gw_close_ret = gzclose(gw);

    (void)version;
    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)inf_init_ret;
    (void)bound_est;
    (void)def_ret;
    (void)comp_len;
    (void)checksum;
    (void)gw_write_ret;
    (void)inf_ret;
    (void)dict_out_len;
    (void)inf_getdict_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gw_close_ret;
    // API sequence test completed successfully
    return 66;
}