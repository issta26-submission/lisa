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
//<ID> 746
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
    Bytef dict[32];
    memset(dict, 'D', sizeof(dict));

    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure (set dictionary for deflate and compress)
    int dict_set_ret = deflateSetDictionary(&def, dict, (uInt)sizeof(dict));
    int deflate_ret = deflate(&def, 4); /* Z_FINISH */
    uLong comp_len = def.total_out;
    int deflate_end_ret = deflateEnd(&def);

    // step 3: Operate (write compressed data to a gz file, adjust params, read one byte)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gzsetparams_ret = gzsetparams(gzw, 9, 1);
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gzclose_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    int gzgetc_val = gzgetc_(gzr);
    int gzclose_r = gzclose(gzr);

    // step 4: Validate / Cleanup (inflate the compressed buffer, exercise inflateBackInit_, cleanup)
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef out_buf[128];
    memset(out_buf, 0, sizeof(out_buf));
    inf.next_out = out_buf;
    inf.avail_out = (uInt)sizeof(out_buf);
    int inflate_ret = inflate(&inf, 0); /* Z_NO_FLUSH */
    int inflate_end_ret = inflateEnd(&inf);

    z_stream infb;
    memset(&infb, 0, sizeof(infb));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int infl_back_init_ret = inflateBackInit_(&infb, 15, window, version, (int)sizeof(z_stream));
    int infl_back_end_ret = inflateBackEnd(&infb);

    free(comp_buf);

    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)dict_set_ret;
    (void)deflate_ret;
    (void)deflate_end_ret;
    (void)comp_len;
    (void)gzsetparams_ret;
    (void)gzwrite_ret;
    (void)gzclose_w;
    (void)gzgetc_val;
    (void)gzclose_r;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)infl_back_init_ret;
    (void)infl_back_end_ret;
    (void)plain_len;
    (void)dict;

    // API sequence test completed successfully
    return 66;
}