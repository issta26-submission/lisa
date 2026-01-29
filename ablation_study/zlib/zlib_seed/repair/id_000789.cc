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
//<ID> 789
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
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

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

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;

    Bytef *decomp_buf = (Bytef *)malloc((size_t)plain_len);
    memset(decomp_buf, 0, (size_t)plain_len);
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;

    // step 3: Operate
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf);
    int inf_ret = inflate(&inf, 0);

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen64(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzf_w);

    gzFile gzf_r = gzopen64(fname, "rb");
    int ch = gzgetc_(gzf_r);
    int gz_close_r_ret = gzclose_r(gzf_r);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    int inf_copy_end_ret = inflateEnd(&inf_copy);

    free(comp_buf);
    free(decomp_buf);

    (void)version;
    (void)plain_len;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)fname;
    (void)gz_wr;
    (void)gz_close_w;
    (void)ch;
    (void)gz_close_r_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}