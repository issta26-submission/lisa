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
//<ID> 546
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
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)sizeof(plain_in);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong compressed_len = def_strm.total_out;
    Bytef dict_buf[64];
    memset(dict_buf, 0, sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict_ret = deflateGetDictionary(&def_strm, dict_buf, &dict_len);

    const char *fname = "test_zlib_api_sequence.tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)sizeof(plain_in));
    off64_t write_off = gzoffset64(gw);
    int gw_close_w = gzclose_w(gw);

    gzFile gr = gzopen(fname, "rb");
    Bytef read_back[128];
    memset(read_back, 0, sizeof(read_back));
    int gr_read = gzread(gr, read_back, (unsigned int)sizeof(read_back));
    off64_t read_off = gzoffset64(gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    uLong crc_orig = crc32(0UL, plain_in, (uInt)sizeof(plain_in));
    uLong crc_read = crc32(0UL, read_back, (uInt)sizeof(read_back));

    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)getdict_ret;
    (void)dict_len;
    (void)gw_write;
    (void)write_off;
    (void)gw_close_w;
    (void)gr_read;
    (void)read_off;
    (void)gr_close;
    (void)def_end_ret;
    (void)crc_orig;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}