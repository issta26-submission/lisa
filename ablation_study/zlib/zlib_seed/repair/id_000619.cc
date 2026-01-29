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
//<ID> 619
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_buf[256];
    memset(plain_buf, 'P', sizeof(plain_buf));
    uLong plain_len = (uLong)sizeof(plain_buf);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_buf, (unsigned int)plain_len);
    int gw_close = gzclose(gw);

    // step 2: Configure
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef read_buf[512];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)plain_len, gr);
    off_t pos_after_read = gztell(gr);

    // step 3: Operate
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    Bytef out_buf[512];
    memset(out_buf, 0, sizeof(out_buf));
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)sizeof(out_buf);
    int reset_keep_ret = inflateResetKeep(&inf_strm);
    int back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate / Cleanup
    int gr_close = gzclose(gr);
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)version;
    (void)gw_write;
    (void)gw_close;
    (void)items_read;
    (void)pos_after_read;
    (void)inf_init_ret;
    (void)reset_keep_ret;
    (void)back_end_ret;
    (void)gr_close;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}