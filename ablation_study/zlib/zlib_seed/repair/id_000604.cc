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
//<ID> 604
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
    memset(plain_in, 'P', sizeof(plain_in));
    uLong source_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)source_len;
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound + 16);
    memset(comp_buf, 0, (size_t)bound + 16);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)((size_t)bound + 16);

    // step 2: Configure
    z_stream copy_strm;
    memset(&copy_strm, 0, sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &def_strm);
    int reset_ret = deflateReset(&copy_strm);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = def_strm.total_out;
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
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)source_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int at_eof_before = gzeof(gr);
    unsigned char read_buf[128];
    memset(read_buf, 0, sizeof(read_buf));
    int gr_read = gzread(gr, read_buf, (unsigned int)sizeof(read_buf));
    int at_eof_after = gzeof(gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int def_end_copy_ret = deflateEnd(&copy_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)gw_write;
    (void)gw_close;
    (void)at_eof_before;
    (void)gr_read;
    (void)at_eof_after;
    (void)gr_close;
    (void)def_end_ret;
    (void)def_end_copy_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}