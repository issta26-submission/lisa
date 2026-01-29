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
//<ID> 750
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
    uLong simple_bound = compressBound(plain_len);
    Bytef *simple_comp = (Bytef *)malloc((size_t)simple_bound);
    memset(simple_comp, 0, (size_t)simple_bound);
    uLongf simple_comp_len = (uLongf)simple_bound;
    int comp_ret = compress(simple_comp, &simple_comp_len, plain, plain_len);

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
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int def_copy_ret = deflateCopy(&def_copy, &def);
    int def_reset_ret = deflateReset(&def_copy);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len2 = def.total_out;
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, out_buf, (unsigned int)comp_len2);
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len2 ? comp_len2 : 1));
    memset(read_buf, 0, (size_t)(comp_len2 ? comp_len2 : 1));
    z_size_t nread = gzfread(read_buf, 1, (z_size_t)comp_len2, gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(simple_comp);
    free(out_buf);
    free(read_buf);
    (void)version;
    (void)plain_len;
    (void)simple_bound;
    (void)simple_comp_len;
    (void)comp_ret;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_copy_ret;
    (void)def_reset_ret;
    (void)def_ret;
    (void)comp_len2;
    (void)gz_wr;
    (void)gz_close_w;
    (void)nread;
    (void)gz_close_r;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}