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
//<ID> 806
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
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound + 1);
    memset(comp_buf, 0, (size_t)out_bound + 1);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    comp_buf[comp_len] = 0;

    // step 3: Operate
    gzFile gzf = gzdopen(1, "wb");
    int gz_put_ret = gzputs(gzf, (const char *)comp_buf);
    int gz_close_ret = gzclose(gzf);
    unsigned long codes_used = inflateCodesUsed(&def);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)plain_len;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_put_ret;
    (void)gz_close_ret;
    (void)codes_used;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}