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
//<ID> 747
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
    Bytef dict[8];
    memset(dict, 'D', sizeof(dict));
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    Bytef out_buf[512];
    memset(out_buf, 0, sizeof(out_buf));
    def.next_out = out_buf;
    def.avail_out = (uInt)sizeof(out_buf);
    int dict_set_ret = deflateSetDictionary(&def, dict, (uInt)sizeof(dict));
    int def_ret = deflate(&def, 4);
    uLong compressed_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = out_buf;
    inf.avail_in = (uInt)compressed_len;
    Bytef decomp[128];
    memset(decomp, 0, sizeof(decomp));
    inf.next_out = decomp;
    inf.avail_out = (uInt)sizeof(decomp);
    z_stream backInf;
    memset(&backInf, 0, sizeof(backInf));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int back_init_ret = inflateBackInit_(&backInf, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    int inf_ret = inflate(&inf, 4);
    int inf_end_ret = inflateEnd(&inf);
    int back_end_ret = inflateBackEnd(&backInf);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_set_ret = gzsetparams(gzw, 1, 0);
    int gz_wr = gzwrite(gzw, plain, (unsigned int)plain_len);
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    int gz_ch = gzgetc_(gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    free(window);
    (void)version;
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)compressed_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)back_init_ret;
    (void)back_end_ret;
    (void)gz_set_ret;
    (void)gz_wr;
    (void)gz_close_w;
    (void)gz_ch;
    (void)gz_close_r;
    (void)plain_len;
    // API sequence test completed successfully
    return 66;
}