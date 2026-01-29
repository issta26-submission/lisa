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
//<ID> 710
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
    memset(plain_in, 'Z', sizeof(plain_in));
    uLong plain_len = (uLong)sizeof(plain_in);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain_in;
    def.avail_in = (uInt)plain_len;

    // step 2: Configure
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

    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, out_buf, (unsigned int)total_out);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    uLong a1 = adler32(0UL, plain_in, (uInt)(plain_len / 2));
    uLong a2 = adler32(0UL, plain_in + (plain_len / 2), (uInt)(plain_len - (plain_len / 2)));
    off64_t part_len = (off64_t)(plain_len / 2);
    uLong combined = adler32_combine64(a1, a2, part_len);

    int def_end_ret = deflateEnd(&def);
    free(out_buf);

    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)pending;
    (void)bits;
    (void)pending_ret;
    (void)total_out;
    (void)gz_w;
    (void)gz_close_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}