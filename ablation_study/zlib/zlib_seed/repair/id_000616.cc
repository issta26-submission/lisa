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
//<ID> 616
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
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[65536];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLong comp_len = (uLong)sizeof(comp_buf);
    int comp_ret = compress2(comp_buf, &comp_len, plain_in, plain_len, 6);

    // step 2: Configure / Operate with gz API
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t pos_before = gztell(gr);
    Bytef read_buf[65536];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)comp_len, gr);
    off_t pos_after = gztell(gr);

    // step 3: Inflate operations
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    Bytef decomp_buf[65536];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int reset_keep_ret = inflateResetKeep(&inf_strm);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int back_end_ret = inflateBackEnd(&inf_strm);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int gr_close = gzclose(gr);

    (void)version;
    (void)comp_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_close;
    (void)pos_before;
    (void)items_read;
    (void)pos_after;
    (void)inf_init_ret;
    (void)reset_keep_ret;
    (void)inf_ret;
    (void)back_end_ret;
    (void)inf_end_ret;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}