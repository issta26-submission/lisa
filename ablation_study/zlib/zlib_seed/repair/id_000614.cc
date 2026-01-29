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
//<ID> 614
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
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[8192];
    memset(comp_buf, 0, sizeof(comp_buf));
    uLongf comp_buf_len = (uLongf)sizeof(comp_buf);
    int comp_ret = compress2(comp_buf, &comp_buf_len, plain_in, plain_len, 6);
    uLong comp_len = (uLong)comp_buf_len;

    // step 2: Configure (inflate)
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[8192];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int inf_reset_keep_ret = inflateResetKeep(&inf_strm);
    int inf_back_end_ret = inflateBackEnd(&inf_strm);

    // step 3: Operate (write/read gz and use gzfread/gztell)
    gzFile gw = gzopen("test_zlib_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, decomp_buf, (unsigned int)inf_strm.total_out);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_sequence.tmp.gz", "rb");
    off_t pos_before = gztell(gr);
    Bytef read_buf[8192];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)inf_strm.total_out, gr);
    off_t pos_after = gztell(gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)version;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_reset_keep_ret;
    (void)inf_back_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)pos_before;
    (void)items_read;
    (void)pos_after;
    (void)gr_close;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}