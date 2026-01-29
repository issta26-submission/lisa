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
//<ID> 745
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[64];
    memset(plain, 'P', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    Bytef dict[16];
    memset(dict, 'D', sizeof(dict));
    uInt dict_len = (uInt)sizeof(dict);

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    int dict_set_ret = deflateSetDictionary(&def, dict, dict_len);
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong compressed_len = def.total_out;
    int def_end_ret = deflateEnd(&def);
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzset_ret = gzsetparams(gzw, 1, 0);
    int gz_wr = gzwrite(gzw, out_buf, (unsigned int)compressed_len);
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");

    // step 3: Operate
    int ch = gzgetc_(gzr);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = out_buf;
    inf.avail_in = (uInt)compressed_len;
    Bytef decomp[64];
    memset(decomp, 0, sizeof(decomp));
    inf.next_out = decomp;
    inf.avail_out = (uInt)sizeof(decomp);
    int inf_ret = inflate(&inf, 0);
    z_stream infb;
    memset(&infb, 0, sizeof(infb));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int infb_init_ret = inflateBackInit_(&infb, 15, window, version, (int)sizeof(z_stream));

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf);
    int infb_end_ret = inflateBackEnd(&infb);
    int gz_close_r = gzclose(gzr);
    free(out_buf);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)out_bound;
    (void)def_ret;
    (void)compressed_len;
    (void)def_end_ret;
    (void)gzset_ret;
    (void)gz_wr;
    (void)gz_close_w;
    (void)ch;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)infb_init_ret;
    (void)inf_end_ret;
    (void)infb_end_ret;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}