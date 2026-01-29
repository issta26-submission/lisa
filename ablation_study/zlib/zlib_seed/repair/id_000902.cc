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
//<ID> 902
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_w_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_w_close = gzclose(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_byte = gzgetc_(gzf_r);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (size_t)(src_len + 64));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_ret = inflate(&inf_strm, 4);
    uLong decomp_len = inf_strm.total_out;

    // step 4: Validate / Cleanup
    int gz_r_close = gzclose(gzf_r);
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_w_ret;
    (void)gz_w_close;
    (void)first_byte;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)gz_r_close;
    (void)def_end_ret;
    (void)inf_end_ret;
    return 66;
    // API sequence test completed successfully
}