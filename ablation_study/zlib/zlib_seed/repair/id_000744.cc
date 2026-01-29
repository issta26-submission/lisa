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
//<ID> 744
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
    Bytef dict[32];
    memset(dict, 'D', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);

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
    int dict_ret = deflateSetDictionary(&def, dict, dict_len);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    Bytef *plain_out = (Bytef *)malloc((size_t)plain_len);
    memset(plain_out, 0, (size_t)plain_len);

    z_stream infb;
    memset(&infb, 0, sizeof(infb));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int infb_ret = inflateBackInit_(&infb, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong compressed_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzw, out_buf, (unsigned int)compressed_len);
    int gz_set_ret = gzsetparams(gzw, 5, 0);
    int gz_close_w = gzclose(gzw);

    int inf_next_in_set = 0;
    inf.next_in = out_buf;
    inf.avail_in = (uInt)compressed_len;
    inf.next_out = plain_out;
    inf.avail_out = (uInt)plain_len;
    int inf_ret = inflate(&inf, 0);
    int inf_end_ret = inflateEnd(&inf);

    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int ch = gzgetc_(gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    free(out_buf);
    free(plain_out);
    free(window);
    (void)version;
    (void)plain_len;
    (void)dict_len;
    (void)def_init_ret;
    (void)out_bound;
    (void)dict_ret;
    (void)inf_init_ret;
    (void)infb_ret;
    (void)def_ret;
    (void)compressed_len;
    (void)def_end_ret;
    (void)gzw;
    (void)gz_wr;
    (void)gz_set_ret;
    (void)gz_close_w;
    (void)inf_next_in_set;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzr;
    (void)ch;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}