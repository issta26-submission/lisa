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
//<ID> 906
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and inflation.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def;
    z_stream inf;
    memset(&def, 0, sizeof(def));
    memset(&inf, 0, sizeof(inf));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    def.next_in = (Bytef *)source;
    def.avail_in = (uInt)src_len;
    uLong comp_bound = deflateBound(&def, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int def_end_ret = deflateEnd(&def);
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (size_t)(src_len + 16));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;
    int inf_end_ret = inflateEnd(&inf);

    // step 4: Validate / Cleanup
    gzFile gw = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gw, decomp_buf, (unsigned int)decomp_len);
    int gz_cl_w = gzclose(gw);
    gzFile gr = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gr);
    int gz_cl_r = gzclose(gr);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)inf_end_ret;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)first_char;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}