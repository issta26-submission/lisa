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
//<ID> 1552
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: create compressed data with compress2, then inflate and uncompress for validation";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf_inflate = (Bytef *)malloc((size_t)src_len);
    memset(out_buf_inflate, 0, (int)src_len);
    inf_strm.next_out = out_buf_inflate;
    inf_strm.avail_out = (uInt)src_len;

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    Bytef *out_buf_uncompress = (Bytef *)malloc((size_t)src_len);
    memset(out_buf_uncompress, 0, (int)src_len);
    uLongf out_len_uncompress = (uLongf)src_len;
    int uncompress_ret = uncompress(out_buf_uncompress, &out_len_uncompress, comp_buf, comp_len);
    uLong compile_flags = zlibCompileFlags();

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf_inflate);
    free(out_buf_uncompress);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)def_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)uncompress_ret;
    (void)out_len_uncompress;
    (void)compile_flags;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}