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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input_buf[256];
    memset(input_buf, 'X', sizeof(input_buf));
    Bytef dict_in[32];
    memset(dict_in, 0xAA, sizeof(dict_in));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int setdict_ret = deflateSetDictionary(&def_strm, dict_in, (uInt)sizeof(dict_in));

    // step 2: Configure
    Bytef comp_buf[1024];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)sizeof(input_buf);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    uLong bound = deflateBound(&def_strm, (uLong)def_strm.avail_in);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_first = crc_table[0];
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off64_t gw_off64 = gzoffset64(gw);
    int gw_close_w_ret = gzclose_w(gw);

    // step 4: Validate / Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    uLong decomp_len = inf_strm.total_out;
    Bytef dict_out[32];
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int getdict_ret = inflateGetDictionary(&inf_strm, dict_out, &dict_out_len);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)ver;
    (void)def_init_ret;
    (void)setdict_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)crc_first;
    (void)gw_write;
    (void)gw_off64;
    (void)gw_close_w_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)getdict_ret;
    (void)dict_out_len;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}