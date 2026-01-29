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
//<ID> 628
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
    Bytef plain_in[256];
    memset(plain_in, 'X', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&def_strm, 4, 8, 16, 32);
    uLong bound = deflateBound(&def_strm, plain_len);

    // step 2: Configure
    Bytef comp_buf[4096];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzoffset(gw);

    // step 4: Validate / Cleanup
    int gw_close = gzclose(gw);
    int def_end_ret = deflateEnd(&def_strm);
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
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}