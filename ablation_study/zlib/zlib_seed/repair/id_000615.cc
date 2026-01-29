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
//<ID> 615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Prepare data and compressor
    const char *version = zlibVersion();
    Bytef plain_in[1024];
    memset(plain_in, 'X', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[65536];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off_t write_pos = gztell(gw);
    int gw_close = gzclose(gw);

    // step 2: Configure reader and read compressed data
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef comp_read_buf[65536];
    memset(comp_read_buf, 0, sizeof(comp_read_buf));
    z_size_t items_read = gzfread((voidp)comp_read_buf, 1, (z_size_t)sizeof(comp_read_buf), gr);
    uLong comp_read_len = (uLong)items_read;
    off_t read_pos = gztell(gr);

    // step 3: Initialize inflater and perform operations including reset and backend cleanup
    Bytef decomp_buf[65536];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_read_buf;
    inf_strm.avail_in = (uInt)comp_read_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int inf_reset_keep_ret = inflateResetKeep(&inf_strm);
    int inf_back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate / Cleanup
    int gr_close = gzclose(gr);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)write_pos;
    (void)gw_close;
    (void)items_read;
    (void)comp_read_len;
    (void)read_pos;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_reset_keep_ret;
    (void)inf_back_end_ret;
    (void)gr_close;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}