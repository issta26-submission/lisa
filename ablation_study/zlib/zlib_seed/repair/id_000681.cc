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
//<ID> 681
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
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef decomp_buf[1024];
    memset(decomp_buf, 0, sizeof(decomp_buf));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;
    int inf_reset_ret = inflateReset(&inf);

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

    // step 3: Operate
    int def_ret = deflate(&def, 4); /* 4 == Z_FINISH */
    int def_end_ret = deflateEnd(&def);
    uLong def_used = def.total_out;

    int inf_ret = inflate(&inf, 4); /* 4 == Z_FINISH */
    int inf_end_ret = inflateEnd(&inf);

    gzFile gz = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_write = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    off64_t gz_off = gzoffset64(gz);
    int gw_close = gzclose(gz);

    // step 4: Validate / Cleanup
    uLong adl_plain = adler32(0UL, plain_in, (uInt)plain_len);
    uLong adl_def = adler32(0UL, def_out_buf, (uInt)def_used);
    uLong adl_comb = adler32_combine64(adl_plain, adl_def, (off64_t)def_used);

    free(comp_buf);
    free(def_out_buf);

    (void)version;
    (void)comp_ret;
    (void)comp_bound;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)def_used;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gw_write;
    (void)gz_off;
    (void)gw_close;
    (void)adl_plain;
    (void)adl_def;
    (void)adl_comb;
    // API sequence test completed successfully
    return 66;
}