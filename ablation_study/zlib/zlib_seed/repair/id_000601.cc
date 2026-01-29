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
//<ID> 601
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[1024];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong source_len = (uLong)sizeof(plain_in);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int init_def = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)source_len;
    Bytef comp_buf[8192];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, source_len);
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def_strm);
    int reset_ret = deflateReset(&def_copy);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gf, comp_buf, (unsigned int)comp_len);
    int eof_flag = gzeof(gf);
    int gz_close_ret = gzclose(gf);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef decomp_buf[1024];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)sizeof(decomp_buf);
    int inf_ret = inflate(&inf, 4);

    // step 4: Validate / Cleanup
    int def_end1 = deflateEnd(&def_strm);
    int def_end2 = deflateEnd(&def_copy);
    int inf_end = inflateEnd(&inf);

    (void)ver;
    (void)init_def;
    (void)bound;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)eof_flag;
    (void)gz_close_ret;
    (void)inf_init;
    (void)inf_ret;
    (void)def_end1;
    (void)def_end2;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}