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
//<ID> 751
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

    uLong comp_bound_simple = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound_simple);
    memset(comp_buf, 0, (size_t)comp_bound_simple);
    uLongf comp_len = (uLongf)comp_bound_simple;
    int comp_ret = compress(comp_buf, &comp_len, plain, plain_len);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;

    // step 2: Configure
    uLong def_out_bound = deflateBound(&def, plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out_buf, 0, (size_t)def_out_bound);
    def.next_out = def_out_buf;
    def.avail_out = (uInt)def_out_bound;

    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);
    int reset_ret = deflateReset(&def_copy);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong def_compressed_len = def.total_out;

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, def_out_buf, (unsigned int)def_compressed_len);
    int gz_close_w = gzclose(gzw);

    gzFile gzr = gzopen(fname, "rb");
    Bytef *read_back = (Bytef *)malloc((size_t)plain_len);
    memset(read_back, 0, (size_t)plain_len);
    z_size_t items_read = gzfread(read_back, 1, (z_size_t)plain_len, gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(comp_buf);
    free(def_out_buf);
    free(read_back);

    (void)version;
    (void)plain_len;
    (void)comp_bound_simple;
    (void)comp_len;
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_out_bound;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)def_compressed_len;
    (void)gz_wr;
    (void)gz_close_w;
    (void)items_read;
    (void)gz_close_r;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}