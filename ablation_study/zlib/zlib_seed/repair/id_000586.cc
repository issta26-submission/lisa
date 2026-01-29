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
//<ID> 586
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = zlibVersion();
    Bytef plain_in[128];
    memset(plain_in, 'A', sizeof(plain_in));
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)sizeof(plain_in);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw_close_ret = gzclose(gw);
    uLong checksum = crc32(0UL, plain_in, (uInt)sizeof(plain_in));

    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */

    // step 4: Validate / Cleanup
    Bytef dict_out[64];
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int inf_getdict_ret = inflateGetDictionary(&inf_strm, dict_out, &dict_out_len);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_close_ret;
    (void)checksum;
    (void)inf_ret;
    (void)dict_out_len;
    (void)inf_getdict_ret;
    (void)def_end_ret;
    (void)inf_end_ret;

    // API sequence test completed successfully
    return 66;
}