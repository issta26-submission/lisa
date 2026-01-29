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
//<ID> 787
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

    // step 2: Configure
    uLong out_bound = deflateBound(&def, plain_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_bound);
    memset(out_buf, 0, (size_t)out_bound);
    def.next_out = out_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = out_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)plain_len);
    memset(decomp_buf, 0, (size_t)plain_len);
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)plain_len;

    // step 3: Operate
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf);
    int inf_ret = inflate(&inf_copy, 0);
    uLong decomp_len = inf_copy.total_out;

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_write = gzopen64(fname, "wb");
    int gz_wr = gzwrite(gzf_write, plain, (unsigned int)plain_len);
    int gz_close_wr = gzclose(gzf_write);

    gzFile gzf_read = gzopen64(fname, "rb");
    int first_char = gzgetc_(gzf_read);
    int gz_close_rd = gzclose_r(gzf_read);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int inf_end_ret = inflateEnd(&inf);
    int inf_copy_end_ret = inflateEnd(&inf_copy);
    free(out_buf);
    free(decomp_buf);

    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_close_wr;
    (void)first_char;
    (void)gz_close_rd;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}