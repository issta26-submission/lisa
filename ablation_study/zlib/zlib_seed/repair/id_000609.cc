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
//<ID> 609
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
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, plain_len);
    Bytef comp_buf[65536];
    memset(comp_buf, 0, sizeof(comp_buf));
    Bytef decomp_buf[65536];
    memset(decomp_buf, 0, sizeof(decomp_buf));

    // step 2: Configure
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def_strm);
    int reset_ret = deflateReset(&def_copy);

    // step 3: Operate
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int eof_flag = gzeof(gr);
    int gr_close = gzclose(gr);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4); /* 4 == Z_FINISH */

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int def_copy_end_ret = deflateEnd(&def_copy);
    int inf_end_ret = inflateEnd(&inf_strm);

    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)reset_ret;
    (void)gw_write;
    (void)gw_close;
    (void)eof_flag;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}