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
//<ID> 748
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
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    Bytef dict[32];
    memset(dict, 'D', sizeof(dict));
    int dict_ret = deflateSetDictionary(&def, dict, (uInt)sizeof(dict));

    // step 2: Configure
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = out_buf;
    inf.avail_in = 0; // will set after compression
    Bytef *decomp = (Bytef *)malloc((size_t)plain_len);
    memset(decomp, 0, (size_t)plain_len);
    inf.next_out = decomp;
    inf.avail_out = (uInt)plain_len;
    z_stream back;
    memset(&back, 0, sizeof(back));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&back, 15, window, version, (int)sizeof(z_stream));
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int setparams_ret = gzsetparams(gzw, 3, 0);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong compressed_len = def.total_out;
    inf.avail_in = (uInt)compressed_len;
    int gz_wr = gzwrite(gzw, out_buf, (unsigned int)compressed_len);
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    int first_char = gzgetc_(gzr);
    int inf_ret = inflate(&inf, 4);
    int back_end_ret = inflateBackEnd(&back);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    int gz_close_r = gzclose(gzr);
    free(out_buf);
    free(decomp);
    free(window);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)dict_ret;
    (void)out_bound;
    (void)def_ret;
    (void)compressed_len;
    (void)gz_wr;
    (void)gz_close_w;
    (void)first_char;
    (void)inf_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}