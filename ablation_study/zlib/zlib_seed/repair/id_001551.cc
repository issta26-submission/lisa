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
//<ID> 1551
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress2 -> uncompress -> inflate -> inflateCodesUsed -> deflate cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong compile_flags = zlibCompileFlags();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    Bytef *uncomp_buf = (Bytef *)malloc((size_t)src_len);
    memset(uncomp_buf, 0, (int)src_len);
    uLongf uncompr_len = (uLongf)src_len;
    int un_ret = uncompress(uncomp_buf, &uncompr_len, comp_buf, (uLong)comp_len);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *inflate_out = (Bytef *)malloc((size_t)src_len);
    memset(inflate_out, 0, (int)src_len);
    inf_strm.next_out = inflate_out;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);

    // step 3: Operate
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(uncomp_buf);
    free(inflate_out);
    (void)compile_flags;
    (void)comp_ret;
    (void)un_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)def_init_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}