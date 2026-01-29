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
//<ID> 778
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
    memset(plain, 'P', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);

    // step 2: Configure
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong def_out_bound = deflateBound(&def, plain_len);
    Bytef *def_out_buf = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out_buf, 0, (size_t)def_out_bound);
    def.next_out = def_out_buf;
    def.avail_out = (uInt)def_out_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);
    unsigned char *window_buf = (unsigned char *)malloc((size_t)(1U << 15));
    memset(window_buf, 0, (size_t)(1U << 15));
    z_stream infl_back;
    memset(&infl_back, 0, sizeof(infl_back));
    int infl_back_init_ret = inflateBackInit_(&infl_back, 15, window_buf, version, (int)sizeof(z_stream));
    gzFile gz = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    int gz_close_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    int infl_back_end_ret = inflateBackEnd(&infl_back);
    free(comp_buf);
    free(def_out_buf);
    free(window_buf);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_out_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)infl_back_init_ret;
    (void)gz_wr;
    (void)gz_close_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)infl_back_end_ret;
    // API sequence test completed successfully
    return 66;
}