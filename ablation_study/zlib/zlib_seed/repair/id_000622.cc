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
//<ID> 622
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
    Bytef plain_in[1024];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 32, 258, 258, 4096);
    uLong bound = deflateBound(&def_strm, plain_len);

    // step 2: Configure
    uLong comp_buf_size = bound + 16;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_buf_size);
    memset(comp_buf, 0, (size_t)comp_buf_size);
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_buf_size;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzoffset(gw);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t offset_read = gzoffset(gr);
    Bytef read_buf[2048];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)comp_len, gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef out_buf[1024];
    memset(out_buf, 0, sizeof(out_buf));
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)sizeof(out_buf);
    int inf_ret = inflate(&inf_strm, 4);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    (void)version;
    (void)compile_flags;
    (void)def_init_ret;
    (void)tune_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)pos_after_write;
    (void)gw_close;
    (void)offset_read;
    (void)items;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}