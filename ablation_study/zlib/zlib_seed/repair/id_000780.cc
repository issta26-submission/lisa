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
//<ID> 780
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

    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);

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
    int def_ret = deflate(&def, 4);

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen64(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_close_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen64(fname, "rb");
    int first_byte = gzgetc_(gzf_r);

    z_stream inf_src;
    memset(&inf_src, 0, sizeof(inf_src));
    int inf_init_ret = inflateInit_(&inf_src, version, (int)sizeof(z_stream));
    inf_src.next_in = comp_buf;
    inf_src.avail_in = (uInt)comp_len;
    z_stream inf_copy;
    memset(&inf_copy, 0, sizeof(inf_copy));
    int copy_ret = inflateCopy(&inf_copy, &inf_src);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_src);
    int inf_copy_end_ret = inflateEnd(&inf_copy);
    int def_end_ret = deflateEnd(&def);
    int gz_close_r_ret = gzclose_r(gzf_r);
    free(comp_buf);
    free(out_buf);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)fname;
    (void)gz_wr;
    (void)gz_close_w;
    (void)first_byte;
    (void)inf_init_ret;
    (void)copy_ret;
    (void)inf_end_ret;
    (void)inf_copy_end_ret;
    (void)def_end_ret;
    (void)gz_close_r_ret;
    // API sequence test completed successfully
    return 66;
}