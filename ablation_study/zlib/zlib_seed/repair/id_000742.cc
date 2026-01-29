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
//<ID> 742
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef input_buf[128];
    memset(input_buf, 'X', sizeof(input_buf));
    uInt input_len = (uInt)sizeof(input_buf);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.next_in = input_buf;
    def.avail_in = input_len;
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    Bytef dictionary[16];
    memset(dictionary, 'D', sizeof(dictionary));
    int dict_set_ret = deflateSetDictionary(&def, dictionary, (uInt)sizeof(dictionary));
    uLong out_bound = deflateBound(&def, (uLong)input_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef out_buf[256];
    memset(out_buf, 0, sizeof(out_buf));
    inf.next_out = out_buf;
    inf.avail_out = (uInt)sizeof(out_buf);
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    int inf_ret = inflate(&inf, 0);
    // prepare an inflate-back stream (separate) to exercise inflateBackInit_
    z_stream infb;
    memset(&infb, 0, sizeof(infb));
    unsigned char window[32];
    memset(window, 0, sizeof(window));
    int infb_init_ret = inflateBackInit_(&infb, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzw = gzopen(fname, "wb");
    int gz_set_ret = gzsetparams(gzw, 9, 1);
    int gz_putc_ret = gzputc(gzw, 'A');
    int gz_close_w = gzclose(gzw);
    gzFile gzr = gzopen(fname, "rb");
    int gz_ch = gzgetc_(gzr);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf);
    int infb_end_ret = inflateBackEnd(&infb);
    free(comp_buf);
    (void)version;
    (void)input_len;
    (void)def_init_ret;
    (void)dict_set_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)infb_init_ret;
    (void)gz_set_ret;
    (void)gz_putc_ret;
    (void)gz_close_w;
    (void)gz_ch;
    (void)gz_close_r;
    (void)inf_end_ret;
    (void)infb_end_ret;
    // API sequence test completed successfully
    return 66;
}