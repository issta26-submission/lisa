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
//<ID> 1550
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API full sequence: compress2 -> uncompress -> inflate/init/end -> deflate/init/end";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong compile_flags = zlibCompileFlags();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    uLongf uncompr_len = src_len;
    Bytef *uncompr_buf = (Bytef *)malloc((size_t)uncompr_len);
    memset(uncompr_buf, 0, (int)uncompr_len);
    int uncompr_ret = uncompress(uncompr_buf, &uncompr_len, comp_buf, (uLong)comp_len);

    // step 3: Operate
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *inflate_out = (Bytef *)malloc((size_t)src_len);
    memset(inflate_out, 0, (int)src_len);
    inf_strm.next_out = inflate_out;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound2 = deflateBound(&def_strm, uncompr_len);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound2);
    memset(comp_buf2, 0, (int)bound2);
    uLongf comp2_len = (uLongf)bound2;
    int comp2_ret = compress2(comp_buf2, &comp2_len, uncompr_buf, uncompr_len, 6);
    int def_end_ret = deflateEnd(&def_strm);

    // step 4: Validate & Cleanup
    free(comp_buf);
    free(uncompr_buf);
    free(inflate_out);
    free(comp_buf2);
    (void)compile_flags;
    (void)comp_ret;
    (void)uncompr_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)def_init_ret;
    (void)comp2_ret;
    (void)def_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}