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
//<ID> 716
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef input_buf[300];
    memset(input_buf, 'Z', sizeof(input_buf));
    uLong input_len = (uLong)sizeof(input_buf);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = input_buf;
    def.avail_in = (uInt)input_len;

    // step 2: Configure
    uLong out_bound = deflateBound(&def, input_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;

    unsigned int pending_before = 0;
    int bits_before = 0;
    int pending_ret_before = deflatePending(&def, &pending_before, &bits_before);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    unsigned int pending_after = 0;
    int bits_after = 0;
    int pending_ret_after = deflatePending(&def, &pending_after, &bits_after);
    uLong total_out = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 4: Validate / Cleanup
    uLong a1 = adler32(0UL, input_buf, 150);
    uLong a2 = adler32(0UL, input_buf + 150, (uInt)(input_len - 150));
    off64_t part_len = (off64_t)(input_len - 150);
    uLong combined = adler32_combine64(a1, a2, part_len);

    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)pending_before;
    (void)bits_before;
    (void)pending_ret_before;
    (void)def_ret;
    (void)pending_after;
    (void)bits_after;
    (void)pending_ret_after;
    (void)total_out;
    (void)def_end_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}