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
//<ID> 593
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
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong source_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[2048];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 2: Configure
    int prime_ret = deflatePrime(&def_strm, 3, 1);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    off64_t gw_pos = gzseek64(gw, 0, 1);
    int gw_close = gzclose(gw);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);

    (void)version;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_pos;
    (void)gw_close;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}