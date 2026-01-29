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
//<ID> 623
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
    Bytef in_buf[512];
    memset(in_buf, 'X', sizeof(in_buf));
    uLong in_len = (uLong)sizeof(in_buf);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, in_len);
    Bytef comp_buf[8192];
    memset(comp_buf, 0, sizeof(comp_buf));

    // step 2: Configure
    def_strm.next_in = in_buf;
    def_strm.avail_in = (uInt)in_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    int tune_ret = deflateTune(&def_strm, 32, 16, 64, 128);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, in_buf, (unsigned int)in_len);
    off_t gw_off = gzoffset(gw);
    int gw_close = gzclose(gw);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    (void)version;
    (void)compile_flags;
    (void)def_init_ret;
    (void)bound;
    (void)tune_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_off;
    (void)gw_close;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}