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
//<ID> 740
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
    memset(plain, 'P', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);
    Bytef dict_sample[32];
    memset(dict_sample, 'D', sizeof(dict_sample));

    // step 2: Configure (deflate with dictionary)
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    int dict_set_ret = deflateSetDictionary(&def, dict_sample, (uInt)sizeof(dict_sample));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    uLong compressed_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 3: Operate (write compressed data to .gz, adjust params, read one byte, then inflate and init inflateBack)
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzw, out_buf, (unsigned int)compressed_len);
    int gz_setp_ret = gzsetparams(gzw, 9, 1);
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    int first_byte = gzgetc_(gzr);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = out_buf;
    inf.avail_in = (uInt)compressed_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)plain_len);
    memset(decomp_buf, 0, (size_t)plain_len);
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;
    int inf_ret = inflate(&inf, 4);
    uLong decompressed_len = inf.total_out;
    int inf_end_ret = inflateEnd(&inf);

    z_stream infb;
    memset(&infb, 0, sizeof(infb));
    unsigned char *window_buf = (unsigned char *)malloc(32768);
    memset(window_buf, 0, 32768);
    int infl_back_init_ret = inflateBackInit_(&infb, 15, window_buf, version, (int)sizeof(z_stream));
    int infl_back_end_ret = inflateBackEnd(&infb);

    // step 4: Validate / Cleanup
    int gz_close_r = gzclose(gzr);
    free(out_buf);
    free(decomp_buf);
    free(window_buf);
    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)out_bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)compressed_len;
    (void)gz_wr;
    (void)gz_setp_ret;
    (void)gz_close_w;
    (void)first_byte;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decompressed_len;
    (void)inf_end_ret;
    (void)infl_back_init_ret;
    (void)infl_back_end_ret;
    (void)gz_close_r;
    // API sequence test completed successfully
    return 66;
}