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
//<ID> 620
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
    Bytef plain[256];
    memset(plain, 'X', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, plain_len);
    int tune_ret = deflateTune(&def_strm, 32, 8, 64, 128);

    // step 2: Configure
    Bytef comp_buf[65536];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_in = plain;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain, (unsigned int)plain_len);
    off_t pos_after_write = gzoffset(gw);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t pos_read_start = gzoffset(gr);
    Bytef read_buf[256];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)plain_len, gr);
    int gr_close = gzclose(gr);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    Bytef decomp_buf[65536];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)version;
    (void)compile_flags;
    (void)def_init_ret;
    (void)bound;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)pos_after_write;
    (void)gw_close;
    (void)pos_read_start;
    (void)items_read;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}