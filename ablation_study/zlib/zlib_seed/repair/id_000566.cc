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
//<ID> 566
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
    Bytef dictionary[32];
    memset(dictionary, 0x5A, sizeof(dictionary));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&def_strm, dictionary, (uInt)sizeof(dictionary));

    // step 2: Configure
    uLong source_len = (uLong)sizeof(input_buf);
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    uLong bound_est = deflateBound(&def_strm, source_len);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
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
    Bytef retrieved_dict[64];
    memset(retrieved_dict, 0, sizeof(retrieved_dict));
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int infl_getdict_ret = inflateGetDictionary(&inf_strm, retrieved_dict, &retrieved_len);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t first_crc = crc_tbl[0];
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)ver;
    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)bound_est;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_off64;
    (void)gw_close_w_ret;
    (void)inf_init_ret;
    (void)infl_getdict_ret;
    (void)retrieved_len;
    (void)first_crc;
    (void)inf_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}