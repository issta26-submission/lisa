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
//<ID> 1568
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateBound -> compress2 -> deflateCopy -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound_compress = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound_compress);
    memset(comp_buf, 0, (int)bound_compress);
    uLongf comp_len = (uLongf)bound_compress;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream def_strm;
    z_stream def_copy;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&def_copy, 0, (int)sizeof(def_copy));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound_deflate = deflateBound(&def_strm, src_len);
    int copy_ret = deflateCopy(&def_copy, &def_strm);

    // step 3: Operate
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int def_copy_end_ret = deflateEnd(&def_copy);
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)bound_compress;
    (void)comp_len;
    (void)comp_ret;
    (void)ver;
    (void)def_init_ret;
    (void)bound_deflate;
    (void)copy_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}