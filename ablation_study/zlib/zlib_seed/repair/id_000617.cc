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
//<ID> 617
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[1024];
    memset(plain_in, 'X', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[65536];
    memset(comp_buf, 0, sizeof(comp_buf));
    Bytef decomp_buf[65536];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    uLong bound_est = deflateBound(&def_strm, plain_len);

    // step 2: Configure
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw_flush = gzflush(gw, 2); /* 2 == Z_FINISH when used as flush code in examples */
    int gw_close = gzclose(gw);

    // step 3: Operate
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t cur_pos = gztell(gr);
    Bytef read_buf[16384];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread((voidp)read_buf, 1, (z_size_t)sizeof(read_buf), gr);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_reset_keep_ret = inflateResetKeep(&inf_strm);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int inf_back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int gr_close = gzclose(gr);

    (void)version;
    (void)bound_est;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_flush;
    (void)gw_close;
    (void)cur_pos;
    (void)items_read;
    (void)inf_init_ret;
    (void)inf_reset_keep_ret;
    (void)inf_ret;
    (void)inf_back_end_ret;
    (void)inf_end_ret;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}