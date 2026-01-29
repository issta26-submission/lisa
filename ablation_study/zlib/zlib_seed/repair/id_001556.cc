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
//<ID> 1556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress2 -> inflate -> uncompress -> record codes used and cleanup with deflateEnd/inflateEnd";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong compile_flags = zlibCompileFlags();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    Bytef *alt_out = (Bytef *)malloc((size_t)src_len);
    memset(alt_out, 0, (int)src_len);
    uLongf alt_out_len = (uLongf)src_len;
    int uncmp_ret = uncompress(alt_out, &alt_out_len, comp_buf, comp_len);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    free(alt_out);
    (void)compile_flags;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)ver;
    (void)inf_init_ret;
    (void)def_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)uncmp_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}