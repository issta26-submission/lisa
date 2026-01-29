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
//<ID> 541
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef plain_in[128];
    memset(plain_in, 'P', sizeof(plain_in));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    strm.next_in = plain_in;
    strm.avail_in = (uInt)sizeof(plain_in);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&strm, 4); /* 4 == Z_FINISH */
    uLong compressed_len = strm.total_out;
    Bytef dict_buf[128];
    memset(dict_buf, 0, sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int dg_ret = deflateGetDictionary(&strm, dict_buf, &dict_len);

    const char *fname = "test_zlib_api_sequence.tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int gw_write = gzwrite(gw, comp_buf, (unsigned int)compressed_len);
    off64_t gw_off = gzoffset64(gw);
    int gw_close_w_ret = gzclose_w(gw);

    // step 4: Validate / Cleanup
    gzFile gr = gzopen(fname, "rb");
    Bytef read_back[512];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    int gr_close_ret = gzclose(gr);
    int def_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)init_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)dg_ret;
    (void)dict_len;
    (void)dict_buf;
    (void)gw_write;
    (void)gw_off;
    (void)gw_close_w_ret;
    (void)gr_read;
    (void)gr_close_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}