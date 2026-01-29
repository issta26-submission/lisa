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
//<ID> 903
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and decompression.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (size_t)sizeof(def_strm));
    memset(&inf_strm, 0, (size_t)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)out_bound;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(decomp_buf, 0, (size_t)src_len);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)src_len;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_cl_ret = gzclose(gzf);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gz_wr_ret;
    (void)gz_cl_ret;
    (void)first_char;
    (void)gz_cl_r;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}