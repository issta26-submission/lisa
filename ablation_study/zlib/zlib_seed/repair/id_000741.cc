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
//<ID> 741
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    Bytef plain[256];
    memset(plain, 'P', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    const char *version = zlibVersion();

    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = plain;
    defstrm.avail_in = (uInt)plain_len;
    Bytef dict_sample[32];
    memset(dict_sample, 'D', sizeof(dict_sample));
    int dict_ret = deflateSetDictionary(&defstrm, dict_sample, (uInt)sizeof(dict_sample));
    uLong out_bound = deflateBound(&defstrm, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    defstrm.next_out = out_buf;
    defstrm.avail_out = (uInt)out_bound;

    z_stream instrm;
    memset(&instrm, 0, sizeof(instrm));
    int inf_init = inflateInit_(&instrm, version, (int)sizeof(z_stream));

    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    // step 2: Configure
    gzFile gzw = gzopen(fname, "wb");
    int gzset_w = gzsetparams(gzw, 6, 0);

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    int def_end = deflateEnd(&defstrm);
    int gz_wr = gzwrite(gzw, out_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzw);

    gzFile gzr = gzopen(fname, "rb");
    int gz_first_char = gzgetc_(gzr);
    int gzset_r = gzsetparams(gzr, 6, 0);

    instrm.next_in = out_buf;
    instrm.avail_in = (uInt)comp_len;
    Bytef *dec_buf = (Bytef *)malloc((size_t)plain_len);
    memset(dec_buf, 0, (size_t)plain_len);
    instrm.next_out = dec_buf;
    instrm.avail_out = (uInt)plain_len;
    int inf_ret = inflate(&instrm, 4);
    int inf_end = inflateEnd(&instrm);

    int back_end = inflateBackEnd(&backstrm);
    int gz_close_r = gzclose(gzr);

    // step 4: Validate / Cleanup
    free(out_buf);
    free(dec_buf);
    (void)def_init;
    (void)dict_ret;
    (void)out_bound;
    (void)def_ret;
    (void)def_end;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_close_w;
    (void)gz_first_char;
    (void)gzset_r;
    (void)inf_init;
    (void)inf_ret;
    (void)inf_end;
    (void)back_init;
    (void)back_end;
    (void)gz_close_r;
    (void)gzset_w;
    (void)plain_len;
    (void)version;
    // API sequence test completed successfully
    return 66;
}