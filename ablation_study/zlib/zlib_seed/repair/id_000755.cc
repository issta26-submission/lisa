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
//<ID> 755
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
    memset(plain, 'Z', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;

    // step 2: Configure
    uLong bound = deflateBound(&def, plain_len);
    Bytef *def_out = (Bytef *)malloc((size_t)bound);
    memset(def_out, 0, (size_t)bound);
    def.next_out = def_out;
    def.avail_out = (uInt)bound;

    int def_reset_ret = deflateReset(&def);

    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int def_copy_ret = deflateCopy(&def_copy, &def);

    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, plain, plain_len);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzw);

    // step 3: Operate
    gzFile gzr = gzopen(fname, "rb");
    z_size_t read_count = gzfread((voidp)comp_buf, (z_size_t)1, (z_size_t)comp_len, gzr);

    int def_ret = deflate(&def, 4);
    uLong deflated_len = def.total_out;

    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(def_out);
    free(comp_buf);

    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)bound;
    (void)def_reset_ret;
    (void)def_copy_ret;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_close_w;
    (void)read_count;
    (void)def_ret;
    (void)deflated_len;
    (void)gz_close_r;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}