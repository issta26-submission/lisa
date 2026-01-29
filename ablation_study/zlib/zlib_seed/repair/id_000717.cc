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
//<ID> 717
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);

    // step 2: Configure
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def, &pending, &bits);
    uLong total_out = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 4: Validate / Cleanup
    uLong half = plain_len / 2;
    uLong a1 = adler32(0UL, plain, (uInt)half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t len1 = (off64_t)half;
    uLong combined = adler32_combine64(a1, a2, len1);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)pending;
    (void)bits;
    (void)pending_ret;
    (void)total_out;
    (void)def_end_ret;
    (void)a1;
    (void)a2;
    (void)len1;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}