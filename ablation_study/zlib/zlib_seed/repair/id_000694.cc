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
//<ID> 694
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

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain_in;
    def.avail_in = (uInt)plain_len;
    uLong def_out_bound = deflateBound(&def, plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out_buf, 0, (size_t)def_out_bound);
    def.next_out = def_out_buf;
    def.avail_out = (uInt)def_out_bound;

    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = def_out_buf;
    inf.avail_in = (uInt)def.total_out;
    Bytef decomp_buf[256];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;
    int prime_ret = inflatePrime(&inf, 3, 5);

    // step 3: Operate
    int inf_ret = inflate(&inf, 4); /* 4 == Z_FINISH */
    int inf_end_ret = inflateEnd(&inf);
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, def_out_buf, (unsigned int)def.total_out);
    int gz_p = gzputs(gz, "extra\n");
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    free(def_out_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_out_bound;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gz_w;
    (void)gz_p;
    (void)gz_close_ret;
    (void)def_end_ret;
    (void)decomp_buf;
    // API sequence test completed successfully
    return 66;
}