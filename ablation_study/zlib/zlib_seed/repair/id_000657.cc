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
//<ID> 657
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
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    Bytef comp_direct[512];
    uLongf comp_direct_len = (uLongf)sizeof(comp_direct);
    int comp_ret = compress(comp_direct, &comp_direct_len, plain_in, plain_len);

    // step 2: Configure
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    Bytef def_out[512];
    memset(def_out, 0, sizeof(def_out));
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)sizeof(def_out);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong def_comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gr);

    // step 4: Validate / Cleanup
    uLong check_adler = adler32(0L, plain_in, (uInt)plain_len);
    int gr_close = gzclose(gr);
    (void)version;
    (void)comp_ret;
    (void)comp_direct_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_comp_len;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)first_char;
    (void)check_adler;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}