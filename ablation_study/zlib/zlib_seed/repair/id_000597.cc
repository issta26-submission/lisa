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
//<ID> 597
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
    Bytef comp_buf[1024];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = plain_in;
    strm.avail_in = (uInt)plain_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)sizeof(comp_buf);

    // step 2: Configure
    int prime_ret = deflatePrime(&strm, 3, 5);
    int def_ret = deflate(&strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = strm.total_out;
    gzFile gf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write = gzwrite(gf, comp_buf, (unsigned int)comp_len);

    // step 3: Operate
    off64_t cur_pos = gzseek64(gf, 0, 1);
    off64_t rewind_pos = gzseek64(gf, 0, 0);
    int extra_write = gzwrite(gf, comp_buf, (unsigned int)((comp_len > 16UL) ? 16U : (unsigned int)comp_len));

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gz_close = gzclose(gf);

    (void)ver;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)cur_pos;
    (void)rewind_pos;
    (void)extra_write;
    (void)def_end_ret;
    (void)gz_close;
    // API sequence test completed successfully
    return 66;
}