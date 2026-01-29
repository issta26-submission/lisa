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
//<ID> 807
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef in_buf[128];
    memset(in_buf, 'A', sizeof(in_buf));
    uLong in_len = (uLong)sizeof(in_buf);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = in_buf;
    def.avail_in = (uInt)in_len;
    uLong out_bound = deflateBound(&def, in_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 3: Operate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)in_len);
    memset(decomp_buf, 0, (size_t)in_len);
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)in_len;
    int inf_ret = inflate(&inf, 0);
    unsigned long codes_used = inflateCodesUsed(&inf);

    // step 4: Validate / Cleanup
    gzFile gz = gzdopen(1, "wb");
    int gz_put_ret = gzputs(gz, "zlib_test");
    int gz_close_ret = gzclose(gz);
    int inf_end_ret = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)in_len;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)gz_put_ret;
    (void)gz_close_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}