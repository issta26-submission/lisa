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
//<ID> 547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef in_buf[256];
    memset(in_buf, 'A', sizeof(in_buf));
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef comp_buf[512];
    memset(comp_buf, 0, sizeof(comp_buf));
    def_strm.next_in = in_buf;
    def_strm.avail_in = (uInt)sizeof(in_buf);
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
    gzFile gw = gzopen64(fname, "wb");
    int gw_write = gzwrite(gw, in_buf, (unsigned int)sizeof(in_buf));
    off64_t gw_off = gzoffset64(gw);
    int gw_close_w_ret = gzclose_w(gw);

    gzFile gr = gzopen64(fname, "rb");
    Bytef read_buf[300];
    memset(read_buf, 0, sizeof(read_buf));
    int gr_read = gzread(gr, read_buf, (unsigned int)sizeof(read_buf));
    int gr_close_ret = gzclose(gr);

    // step 4: Validate / Cleanup
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    uLongf decomp_len = (uLongf)sizeof(decomp_buf);
    int uncompress_ret = uncompress(decomp_buf, &decomp_len, comp_buf, compressed_len);
    int def_end = deflateEnd(&def_strm);

    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)getdict_ret;
    (void)dict_len;
    (void)gw_write;
    (void)gw_off;
    (void)gw_close_w_ret;
    (void)gr_read;
    (void)gr_close_ret;
    (void)uncompress_ret;
    (void)decomp_len;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}