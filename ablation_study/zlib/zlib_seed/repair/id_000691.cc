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
//<ID> 691
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

    // step 2: Operate (compress)
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 3: Configure and use inflatePrime then inflate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    inf.next_out = plain_in;
    inf.avail_out = (uInt)plain_len;
    int prime_ret = inflatePrime(&inf, 3, 5);
    int inf_ret = inflate(&inf, 4); /* 4 == Z_FINISH */
    int inf_end_ret = inflateEnd(&inf);

    // step 4: Validate / Cleanup
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    int gz_p = gzputs(gz, "TAIL");
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)def_bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)prime_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gz_w;
    (void)gz_p;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}