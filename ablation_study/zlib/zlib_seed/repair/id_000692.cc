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
//<ID> 692
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

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain_in;
    def.avail_in = (uInt)plain_len;
    uLong def_bound = deflateBound(&def, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)def_bound);
    memset(comp_buf, 0, (size_t)def_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)def_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    uLong comp_used = def.total_out;
    int def_end_ret = deflateEnd(&def);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_used;
    Bytef decomp_buf[512];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;

    // step 3: Operate
    int prime_ret = inflatePrime(&inf, 3, 1);
    int inf_ret = inflate(&inf, 4);
    int inf_end_ret = inflateEnd(&inf);

    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w_ret = gzwrite(gz, comp_buf, (unsigned int)comp_used);
    int gz_puts_ret = gzputs(gz, "gzputs_line\n");
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)def_bound;
    (void)def_ret;
    (void)comp_used;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)decomp_buf;
    (void)gz_w_ret;
    (void)gz_puts_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}