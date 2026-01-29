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
//<ID> 611
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[256];
    memset(plain_in, 'X', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);

    // step 2: Configure
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef read_buf[256];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)plain_len, gr);
    off_t cur_pos = gztell(gr);

    // step 3: Operate (inflate back interface lifecycle)
    unsigned char window_buf[32768];
    memset(window_buf, 0, sizeof(window_buf));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int ib_init_ret = inflateBackInit_(&inf_strm, 15, window_buf, version, (int)sizeof(z_stream));
    int reset_keep_ret = inflateResetKeep(&inf_strm);
    int back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate / Cleanup
    int gr_close = gzclose(gr);

    (void)version;
    (void)gw_write;
    (void)gw_close;
    (void)items_read;
    (void)cur_pos;
    (void)ib_init_ret;
    (void)reset_keep_ret;
    (void)back_end_ret;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}