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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[256];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uLong bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);

    // step 2: Configure
    z_stream def_src;
    memset(&def_src, 0, sizeof(def_src));
    int def_init_ret = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    def_src.next_in = plain;
    def_src.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def_src, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def_src.next_out = out_buf;
    def_src.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def_src, 4);
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def_src);

    // step 3: Operate
    gzFile gzf = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzf);
    z_stream infb;
    memset(&infb, 0, sizeof(infb));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int ib_init_ret = inflateBackInit_(&infb, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate / Cleanup
    int ib_end_ret = inflateBackEnd(&infb);
    int def_end_ret = deflateEnd(&def_src);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(comp_buf);
    free(out_buf);
    free(window);

    (void)version;
    (void)plain_len;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)gz_wr;
    (void)gz_close_w;
    (void)ib_init_ret;
    (void)ib_end_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}