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
//<ID> 548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef input_buf[128];
    memset(input_buf, 'A', sizeof(input_buf));
    Bytef dict_in[16];
    memset(dict_in, 0x5A, sizeof(dict_in));
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int setdict_ret = deflateSetDictionary(&strm, dict_in, (uInt)sizeof(dict_in));

    // step 2: Configure
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    strm.next_in = input_buf;
    strm.avail_in = (uInt)sizeof(input_buf);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&strm, 4); /* 4 == Z_FINISH */
    uLong comp_len = strm.total_out;
    Bytef dict_out[32];
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int getdict_ret = deflateGetDictionary(&strm, dict_out, &dict_out_len);

    // step 4: Validate / Cleanup
    gzFile gw = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gw_write = gzwrite(gw, input_buf, (unsigned int)sizeof(input_buf));
    off64_t gw_off64 = gzoffset64(gw);
    int gw_close_w_ret = gzclose_w(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    Bytef read_back[128];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    int gr_close = gzclose(gr);
    int def_end_ret = deflateEnd(&strm);

    (void)ver;
    (void)init_ret;
    (void)setdict_ret;
    (void)def_ret;
    (void)comp_len;
    (void)getdict_ret;
    (void)dict_out_len;
    (void)gw_write;
    (void)gw_off64;
    (void)gw_close_w_ret;
    (void)gr_read;
    (void)gr_close;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}