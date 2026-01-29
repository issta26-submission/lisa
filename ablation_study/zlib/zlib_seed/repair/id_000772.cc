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
//<ID> 772
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
    def.next_in = comp_buf;
    def.avail_in = (uInt)comp_len;
    uLong out_bound = deflateBound(&def, comp_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);

    // step 3: Operate
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_back_init_ret = inflateBackInit_(&inf, 15, window, version, (int)sizeof(z_stream));
    gzFile gzf = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    int inf_back_end_ret = inflateBackEnd(&inf);
    free(comp_buf);
    free(out_buf);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)inf_back_init_ret;
    (void)gz_wr;
    (void)gz_close_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)inf_back_end_ret;
    // API sequence test completed successfully
    return 66;
}