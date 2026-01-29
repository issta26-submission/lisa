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
//<ID> 801
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
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong bound = deflateBound(&def, plain_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    def.next_out = comp;
    def.avail_out = (uInt)bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;

    // step 3: Operate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp = (Bytef *)malloc((size_t)plain_len);
    memset(decomp, 0, (size_t)plain_len);
    inf.next_out = decomp;
    inf.avail_out = (uInt)plain_len;
    int inf_ret = inflate(&inf, 0);
    unsigned long codes_used = inflateCodesUsed(&inf);

    gzFile gz = gzdopen(1, "wb");
    int gz_puts_ret = gzputs(gz, "zlib_gzdopen_test\n");
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    free(comp);
    free(decomp);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)gz_puts_ret;
    (void)gz_close_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}