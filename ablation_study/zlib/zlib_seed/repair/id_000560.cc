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
//<ID> 560
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'A', sizeof(plain_in));
    Bytef dict[32];
    memset(dict, 0x5A, sizeof(dict));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&def_strm, dict, (uInt)sizeof(dict));
    uLong bound = deflateBound(&def_strm, (uLong)sizeof(plain_in));
    size_t comp_buf_size = (size_t)bound + 16;
    Bytef *comp_buf = (Bytef *)malloc(comp_buf_size);

    // step 2: Configure
    memset(comp_buf, 0, comp_buf_size);
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)sizeof(plain_in);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_buf_size;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;

    // step 4: Validate / Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    int inf_setdict_ret = inflateSetDictionary(&inf_strm, dict, (uInt)sizeof(dict));
    Bytef dict_out[32];
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int inf_getdict_ret = inflateGetDictionary(&inf_strm, dict_out, &dict_out_len);
    const z_crc_t *crc_tbl = get_crc_table();
    z_crc_t crc0 = crc_tbl[0];
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off64_t gw_off64 = gzoffset64(gw);
    int gw_close_w_ret = gzclose_w(gw);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);

    (void)ver;
    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)bound;
    (void)comp_buf_size;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_setdict_ret;
    (void)inf_getdict_ret;
    (void)dict_out_len;
    (void)crc0;
    (void)gw_write;
    (void)gw_off64;
    (void)gw_close_w_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}