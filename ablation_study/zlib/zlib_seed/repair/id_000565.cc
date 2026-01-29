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
//<ID> 565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input_buf[128];
    memset(input_buf, 'A', sizeof(input_buf));
    Bytef preset_dict[16];
    memset(preset_dict, 0x5A, sizeof(preset_dict));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int setdict_ret = deflateSetDictionary(&def_strm, preset_dict, (uInt)sizeof(preset_dict));

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, (uLong)sizeof(input_buf));
    Bytef * comp_buf = new Bytef[(size_t)bound];
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)sizeof(input_buf);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Validate / Cleanup
    const z_crc_t * crc_tbl = get_crc_table();
    z_crc_t crc_first = crc_tbl[0];

    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, input_buf, (unsigned int)sizeof(input_buf));
    off64_t gw_off64 = gzoffset64(gw);
    int gw_close_w_ret = gzclose_w(gw);

    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef read_back[128];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    off64_t gr_off64 = gzoffset64(gr);
    int gr_close = gzclose(gr);

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
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int igd_ret = inflateGetDictionary(&inf_strm, dict_out, &dict_out_len);
    int inf_end_ret = inflateEnd(&inf_strm);

    delete [] comp_buf;

    (void)ver;
    (void)def_init_ret;
    (void)setdict_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)crc_first;
    (void)gw_write;
    (void)gw_off64;
    (void)gw_close_w_ret;
    (void)gr_read;
    (void)gr_off64;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)igd_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}