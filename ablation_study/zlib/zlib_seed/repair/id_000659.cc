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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[1024];
    memset(plain_in, 'A', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);

    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, plain_in, plain_len);

    // step 2: Configure
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = comp_buf;
    def_strm.avail_in = (uInt)comp_len;
    uLong comp2_bound = deflateBound(&def_strm, (uLong)comp_len);
    Bytef *comp2_buf = (Bytef *)malloc((size_t)comp2_bound);
    memset(comp2_buf, 0, (size_t)comp2_bound);
    def_strm.next_out = comp2_buf;
    def_strm.avail_out = (uInt)comp2_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); /* 4 == Z_FINISH */
    uLong comp2_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gw, comp2_buf, (unsigned int)comp2_len);
    int gw_close = gzclose(gw);

    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int first_byte = gzgetc(gr);
    int gr_close = gzclose(gr);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(comp2_buf);
    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp2_bound;
    (void)comp2_len;
    (void)def_end_ret;
    (void)gw_write;
    (void)gw_close;
    (void)first_byte;
    (void)gr_close;
    // API sequence test completed successfully
    return 66;
}