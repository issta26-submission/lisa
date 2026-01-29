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
//<ID> 777
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
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
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def);

    // step 3: Operate
    z_stream inf_back;
    memset(&inf_back, 0, sizeof(inf_back));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int ib_init_ret = inflateBackInit_(&inf_back, 15, window, version, (int)sizeof(z_stream));
    inf_back.next_in = comp_buf;
    inf_back.avail_in = (uInt)comp_len;
    inf_back.next_out = out_buf;
    inf_back.avail_out = (uInt)out_bound;

    // step 4: Validate / Cleanup
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen64(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_cl = gzclose(gzf);
    int ib_end_ret = inflateBackEnd(&inf_back);
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(comp_buf);
    free(out_buf);
    free(window);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)ib_init_ret;
    (void)gz_wr;
    (void)gz_cl;
    (void)ib_end_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}