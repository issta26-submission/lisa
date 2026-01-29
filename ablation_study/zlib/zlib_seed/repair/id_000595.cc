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
//<ID> 595
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[8192];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 2: Configure
    int prime_ret = deflatePrime(&def_strm, 1, 1);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gf = gzopen64("test_zlib_api_sequence_out.gz", "wb");
    int gw_write = gzwrite(gf, comp_buf, (unsigned int)comp_len);
    off64_t pos_now = gzseek64(gf, 0, 1);
    off64_t pos_rewind = gzseek64(gf, 0, 0);
    int gw_close = gzclose(gf);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_write;
    (void)pos_now;
    (void)pos_rewind;
    (void)gw_close;
    // API sequence test completed successfully
    return 66;
}