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
//<ID> 605
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
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong source_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)source_len;

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    memset(comp_buf2, 0, (size_t)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def_strm);
    def_copy.next_out = comp_buf2;
    def_copy.avail_out = (uInt)bound;
    int reset_ret = deflateReset(&def_strm);

    // step 3: Operate
    int def_ret_main = deflate(&def_strm, 4);
    uLong comp_len_main = def_strm.total_out;
    int def_ret_copy = deflate(&def_copy, 4);
    uLong comp_len_copy = def_copy.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)comp_len_main);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int eof_flag = gzeof(gr);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len_main;
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Validate / Cleanup
    int def_end_main = deflateEnd(&def_strm);
    int def_end_copy = deflateEnd(&def_copy);
    int inf_end = inflateEnd(&inf_strm);
    int gr_close = gzclose(gr);
    free(comp_buf);
    free(comp_buf2);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret_main;
    (void)comp_len_main;
    (void)def_ret_copy;
    (void)comp_len_copy;
    (void)gw_write;
    (void)gw_close;
    (void)eof_flag;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end_main;
    (void)def_end_copy;
    (void)inf_end;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}