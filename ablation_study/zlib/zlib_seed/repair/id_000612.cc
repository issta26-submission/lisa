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
//<ID> 612
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[512];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[4096];
    memset(comp_buf, 0, sizeof(comp_buf));
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Configure (use gzFile read/write APIs)
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t gz_pos = gztell(gr);
    Bytef gz_read_buf[512];
    memset(gz_read_buf, 0, sizeof(gz_read_buf));
    z_size_t gz_items = gzfread(gz_read_buf, 1, (z_size_t)sizeof(gz_read_buf), gr);
    int gr_close = gzclose(gr);

    // step 3: Operate (inflate in-memory compressed data and exercise inflateResetKeep)
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int inf_reset_keep_ret = inflateResetKeep(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate / Cleanup (exercise inflateBackInit_ and inflateBackEnd)
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char window[(1<<15)];
    memset(window, 0, sizeof(window));
    int back_init_ret = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));
    int back_end_ret = inflateBackEnd(&back_strm);

    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)gz_pos;
    (void)gz_items;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_reset_keep_ret;
    (void)inf_end_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    // API sequence test completed successfully
    return 66;
}