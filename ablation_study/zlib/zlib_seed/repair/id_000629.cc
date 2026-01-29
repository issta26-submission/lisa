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
//<ID> 629
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    Bytef plain_in[512];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 32, 16, 64, 2048);
    uLong bound = deflateBound(&def_strm, plain_len);
    Bytef comp_buf[65536];
    memset(comp_buf, 0, sizeof(comp_buf));
    Bytef decomp_buf[65536];
    memset(decomp_buf, 0, sizeof(decomp_buf));

    // step 2: Configure
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef read_buf[512];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)plain_len, gr);
    off_t pos_after_read = gzoffset(gr);

    // step 3: Operate
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gr_close = gzclose(gr);
    (void)version;
    (void)compile_flags;
    (void)def_init_ret;
    (void)tune_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_close;
    (void)items_read;
    (void)pos_after_read;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}