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
//<ID> 587
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = zlibVersion();
    Bytef input_buf[128];
    memset(input_buf, 'X', sizeof(input_buf));
    uLong input_len = (uLong)sizeof(input_buf);
    uLong crc_val = crc32(0L, input_buf, (uInt)input_len);
    Bytef comp_buf[1024];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress2(comp_buf, &comp_len, input_buf, input_len, 6);

    // step 2: Configure
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw_close_w_ret = gzclose_w(gw);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    Bytef dict_out[64];
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int inf_getdict_ret = inflateGetDictionary(&inf_strm, dict_out, &dict_out_len);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = decomp_buf;
    def_strm.avail_in = (uInt)inf_strm.total_out;
    Bytef out_comp[1024];
    memset(out_comp, 0, sizeof(out_comp));
    def_strm.next_out = out_comp;
    def_strm.avail_out = (uInt)sizeof(out_comp);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong def_comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)version;
    (void)crc_val;
    (void)comp_ret;
    (void)gw_write;
    (void)gw_close_w_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)dict_out_len;
    (void)inf_getdict_ret;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_comp_len;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}