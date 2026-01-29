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
//<ID> 1553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: prepare compressed data with compress2, then inflate + uncompress and inspect codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    z_stream inf_strm;
    z_stream def_strm;
    memset(&inf_strm, 0, (int)sizeof(z_stream));
    memset(&def_strm, 0, (int)sizeof(z_stream));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    Bytef *uncmp_buf = (Bytef *)malloc((size_t)src_len);
    memset(uncmp_buf, 0, (int)src_len);
    uLongf uncmp_len = (uLongf)src_len;
    uLong compile_flags = zlibCompileFlags();

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int uncmp_ret = uncompress(uncmp_buf, &uncmp_len, comp_buf, (uLong)comp_len);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(out_buf);
    free(uncmp_buf);
    (void)ver;
    (void)comp_ret;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)compile_flags;
    (void)inf_ret;
    (void)codes_used;
    (void)uncmp_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}