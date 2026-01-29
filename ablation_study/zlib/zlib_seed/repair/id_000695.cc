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
//<ID> 695
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain_in[300];
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;

    // step 2: Configure
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain_in;
    def.avail_in = (uInt)plain_len;
    def.next_out = comp_buf;
    def.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    int def_end_ret = deflateEnd(&def);
    uLong produced = def.total_out;
    comp_len = (uLongf)produced;

    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, comp_buf, (unsigned int)produced);
    int gz_p = gzputs(gz, "gzputs-footer\n");
    int gz_close_ret = gzclose(gz);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)produced;
    Bytef decomp_buf[300];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;
    int prime_ret = inflatePrime(&inf, 3, 1);
    int inf_ret = inflate(&inf, 4);
    int inf_end_ret = inflateEnd(&inf);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)comp_bound;
    (void)comp_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)produced;
    (void)gz_w;
    (void)gz_p;
    (void)gz_close_ret;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)decomp_buf;
    // API sequence test completed successfully
    return 66;
}