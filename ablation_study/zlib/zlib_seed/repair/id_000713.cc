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
//<ID> 713
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef in_buf[256];
    memset(in_buf, 'Z', sizeof(in_buf));
    uLong in_len = (uLong)sizeof(in_buf);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = in_buf;
    def.avail_in = (uInt)in_len;

    // step 2: Configure
    uLong out_bound = deflateBound(&def, in_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret_first = deflate(&def, 0);
    unsigned int pending = 0;
    int bits = 0;
    int pend_ret = deflatePending(&def, &pending, &bits);
    int def_ret_final = deflate(&def, 4);
    uLong total_out = def.total_out;
    int def_end_ret = deflateEnd(&def);

    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_w = gzwrite(gz, out_buf, (unsigned int)total_out);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    uLong a1 = adler32(0UL, in_buf, (uInt)(in_len / 2));
    uLong a2 = adler32(0UL, in_buf + (in_len / 2), (uInt)(in_len - in_len / 2));
    off64_t part_len = (off64_t)(in_len / 2);
    uLong combined = adler32_combine64(a1, a2, part_len);

    free(out_buf);
    (void)version;
    (void)init_ret;
    (void)out_bound;
    (void)def_ret_first;
    (void)pend_ret;
    (void)bits;
    (void)def_ret_final;
    (void)def_end_ret;
    (void)total_out;
    (void)gz_w;
    (void)gz_close_ret;
    (void)a1;
    (void)a2;
    (void)part_len;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}