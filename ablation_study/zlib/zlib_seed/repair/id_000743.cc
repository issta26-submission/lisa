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
//<ID> 743
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
    Bytef dict_sample[32];
    memset(dict_sample, 'D', sizeof(dict_sample));
    z_stream def;
    z_stream inf;
    z_stream back;
    memset(&def, 0, sizeof(def));
    memset(&inf, 0, sizeof(inf));
    memset(&back, 0, sizeof(back));

    // step 2: Configure
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    int dict_set_ret = deflateSetDictionary(&def, dict_sample, (uInt)sizeof(dict_sample));
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_set_ret = gzsetparams(gzw, 9, 1);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);
    int gz_wr = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    int first_byte = gzgetc_(gzr);

    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)plain_len);
    memset(out_buf, 0, (size_t)plain_len);
    inf.next_out = out_buf;
    inf.avail_out = (uInt)plain_len;

    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&back, 15, window, version, (int)sizeof(z_stream));
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;
    int inf_end_ret = inflateEnd(&inf);
    int back_end_ret = inflateBackEnd(&back);

    // step 4: Validate / Cleanup
    int gz_close_r = gzclose(gzr);
    free(comp_buf);
    free(out_buf);
    free(window);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)out_bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_close_w;
    (void)first_byte;
    (void)inf_init_ret;
    (void)back_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)inf_end_ret;
    (void)back_end_ret;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}