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
//<ID> 719
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
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    // step 2: Configure
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
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
    uInt half = (uInt)(plain_len / 2UL);
    uLong a1 = adler32(0UL, plain, half);
    uLong a2 = adler32(0UL, plain + half, (uInt)(plain_len - half));
    off64_t part_len = (off64_t)half;
    uLong combined = adler32_combine64(a1, a2, part_len);

    free(out_buf);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)def_ret;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)total_out;
    (void)def_end_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}