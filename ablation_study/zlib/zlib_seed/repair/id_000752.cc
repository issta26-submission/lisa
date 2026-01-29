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
//<ID> 752
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
    uLong bound1 = compressBound(plain_len);
    Bytef *comp_buf1 = (Bytef *)malloc((size_t)bound1);
    memset(comp_buf1, 0, (size_t)bound1);
    uLongf comp1_len = (uLongf)bound1;
    int comp_ret = compress(comp_buf1, &comp1_len, plain, plain_len);

    // step 2: Configure
    z_stream src;
    memset(&src, 0, sizeof(src));
    int def_init_ret = deflateInit_(&src, 6, version, (int)sizeof(z_stream));
    src.next_in = plain;
    src.avail_in = (uInt)plain_len;
    uLong out_bound2 = deflateBound(&src, plain_len);
    Bytef *out_buf2 = (Bytef *)malloc((size_t)out_bound2);
    memset(out_buf2, 0, (size_t)out_bound2);
    src.next_out = out_buf2;
    src.avail_out = (uInt)out_bound2;

    // step 3: Operate
    int def_ret = deflate(&src, 4);
    uLong def_comp_len = src.total_out;
    z_stream copy;
    memset(&copy, 0, sizeof(copy));
    int copy_ret = deflateCopy(&copy, &src);
    int reset_ret = deflateReset(&copy);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, comp_buf1, (unsigned int)comp1_len);
    int gz_wr2 = gzwrite(gzw, out_buf2, (unsigned int)def_comp_len);
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    z_size_t read_items = gzfread((voidp)out_buf2, (z_size_t)1, (z_size_t)comp1_len, gzr);

    // step 4: Validate / Cleanup
    int gz_close_r = gzclose(gzr);
    int src_end = deflateEnd(&src);
    int copy_end = deflateEnd(&copy);
    free(comp_buf1);
    free(out_buf2);
    (void)version;
    (void)plain_len;
    (void)bound1;
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_comp_len;
    (void)copy_ret;
    (void)reset_ret;
    (void)gz_wr;
    (void)gz_wr2;
    (void)gz_close_w;
    (void)read_items;
    (void)gz_close_r;
    (void)src_end;
    (void)copy_end;
    // API sequence test completed successfully
    return 66;
}