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
//<ID> 697
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[128];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef decomp[128];
    memset(decomp, 0, sizeof(decomp));
    inf.next_out = decomp;
    inf.avail_out = (uInt)plain_len;
    int inf_prime_ret = inflatePrime(&inf, 0, 0);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong def_out_bound = deflateBound(&def, plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out_buf, 0, (size_t)def_out_bound);
    def.next_out = def_out_buf;
    def.avail_out = (uInt)def_out_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    int def_end_ret = deflateEnd(&def);
    int inf_ret = inflate(&inf, 4);
    int inf_end_ret = inflateEnd(&inf);

    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_puts_ret = gzputs(gz, "zlib-header\n");
    int gz_write_ret = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(def_out_buf);
    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_prime_ret;
    (void)decomp;
    (void)def_init_ret;
    (void)def_out_bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gz_puts_ret;
    (void)gz_write_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}