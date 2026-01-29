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
//<ID> 613
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
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);

    // step 2: Configure / Read back
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t pos_before = gztell(gr);
    Bytef read_buf[256];
    memset(read_buf, 0, sizeof(read_buf));
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)sizeof(read_buf), gr);
    off_t pos_after = gztell(gr);
    int gr_close_mid = gzclose(gr);

    // step 3: Operate (compress then inflate)
    uLongf comp_buf_len = compressBound((uLong)items_read);
    Bytef comp_buf[1024];
    memset(comp_buf, 0, sizeof(comp_buf));
    int comp_ret = compress2(comp_buf, &comp_buf_len, read_buf, (uLong)items_read, 6);
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_buf_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Validate / Cleanup
    int reset_keep_ret = inflateResetKeep(&inf_strm);
    int back_end_ret = inflateBackEnd(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)version;
    (void)gw_write;
    (void)gw_close;
    (void)pos_before;
    (void)items_read;
    (void)pos_after;
    (void)gr_close_mid;
    (void)comp_ret;
    (void)comp_buf_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)reset_keep_ret;
    (void)back_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}