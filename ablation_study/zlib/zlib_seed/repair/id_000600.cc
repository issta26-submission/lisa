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
//<ID> 600
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[512];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[65536];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, plain_len);

    // step 2: Configure
    int def_reset_ret = deflateReset(&def_strm);
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    z_stream copy_strm;
    memset(&copy_strm, 0, sizeof(copy_strm));
    int def_copy_ret = deflateCopy(&copy_strm, &def_strm);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    gzFile gf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gf, comp_buf, (unsigned int)comp_len);
    int eof_flag = gzeof(gf);

    // step 4: Validate / Cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */
    int def_end_ret = deflateEnd(&def_strm);
    int copy_end_ret = deflateEnd(&copy_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gz_close_ret = gzclose(gf);

    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_reset_ret;
    (void)def_copy_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)eof_flag;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)copy_end_ret;
    (void)inf_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}