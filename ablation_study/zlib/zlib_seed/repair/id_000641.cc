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
//<ID> 641
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
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[2048];
    memset(comp_buf, 0, sizeof(comp_buf));
    Bytef decomp_buf[1024];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int tune_ret = deflateTune(&def_strm, 32, 16, 64, 1024);
    int params_ret = deflateParams(&def_strm, 6, 0);
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;

    // step 3: Operate
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit2_(&inf_strm, 15, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    uLong decomp_len = inf_strm.total_out;

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)version;
    (void)def_init_ret;
    (void)tune_ret;
    (void)params_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}