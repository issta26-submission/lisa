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
//<ID> 607
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
    uLong source_len = (uLong)sizeof(plain_in);
    Bytef comp_buf[8192];
    memset(comp_buf, 0, sizeof(comp_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);
    uLong bound = deflateBound(&def_strm, source_len);

    // step 2: Configure
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def_strm);
    int reset_ret = deflateReset(&def_copy);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)source_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    int eof_before = gzeof(gr);
    Bytef gz_read_buf[512];
    memset(gz_read_buf, 0, sizeof(gz_read_buf));
    int gr_read = gzread(gr, gz_read_buf, (unsigned int)sizeof(gz_read_buf));
    int gr_close = gzclose(gr);

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
    int inf_ret = inflate(&inf_strm, 4);
    int def_end = deflateEnd(&def_strm);
    int def_copy_end = deflateEnd(&def_copy);
    int inf_end = inflateEnd(&inf_strm);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gw_write;
    (void)gw_close;
    (void)eof_before;
    (void)gr_read;
    (void)gr_close;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end;
    (void)def_copy_end;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}