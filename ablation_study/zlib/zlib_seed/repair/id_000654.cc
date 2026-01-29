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
//<ID> 654
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
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);

    // step 2: Configure
    uLong comp_bound1 = compressBound(plain_len);
    uLongf comp_len1 = (uLongf)comp_bound1;
    Bytef *comp_buf1 = (Bytef *)malloc((size_t)comp_bound1);
    memset(comp_buf1, 0, (size_t)comp_bound1);
    int comp_ret = compress(comp_buf1, &comp_len1, plain_in, plain_len);

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong comp_bound2 = deflateBound(&def_strm, plain_len);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)comp_bound2);
    memset(comp_buf2, 0, (size_t)comp_bound2);
    def_strm.next_in = plain_in;
    def_strm.avail_in = (uInt)plain_len;
    def_strm.next_out = comp_buf2;
    def_strm.avail_out = (uInt)comp_bound2;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len2 = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, plain_in, (unsigned int)plain_len);
    int gw_close = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc(gr);

    // step 4: Validate / Cleanup
    int gr_close = gzclose(gr);
    (void)version;
    (void)comp_ret;
    (void)comp_len1;
    (void)comp_len2;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)first_char;
    free(comp_buf1);
    free(comp_buf2);
    // API sequence test completed successfully
    return 66;
}