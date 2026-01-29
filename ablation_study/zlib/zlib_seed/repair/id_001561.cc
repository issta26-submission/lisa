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
//<ID> 1561
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateBound -> compress -> deflateCopy -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong cb_compress = compressBound(src_len);
    z_stream def_stream;
    memset(&def_stream, 0, (int)sizeof(def_stream));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_stream, 6, ver, (int)sizeof(z_stream));
    uLong def_bound_orig = deflateBound(&def_stream, src_len);
    z_stream copy_stream;
    memset(&copy_stream, 0, (int)sizeof(copy_stream));
    int def_copy_ret = deflateCopy(&copy_stream, &def_stream);
    uLong def_bound_copy = deflateBound(&copy_stream, src_len);
    uLong total_bound = cb_compress + def_bound_orig + def_bound_copy;
    Bytef *comp_buf = (Bytef *)malloc((size_t)total_bound);
    memset(comp_buf, 0, (int)total_bound);

    // step 2: Configure
    uLongf comp_len = (uLongf)cb_compress;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);
    z_stream inf_stream;
    memset(&inf_stream, 0, (int)sizeof(inf_stream));
    int inf_init_ret = inflateInit_(&inf_stream, ver, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_stream.next_in = comp_buf;
    inf_stream.avail_in = (uInt)comp_len;
    inf_stream.next_out = out_buf;
    inf_stream.avail_out = (uInt)src_len;

    // step 3: Operate
    int inf_ret = inflate(&inf_stream, 0);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_stream);
    int def_end_ret = deflateEnd(&def_stream);
    int def_copy_end_ret = deflateEnd(&copy_stream);
    free(comp_buf);
    free(out_buf);
    (void)ver;
    (void)def_init_ret;
    (void)def_bound_orig;
    (void)def_copy_ret;
    (void)def_bound_copy;
    (void)total_bound;
    (void)comp_len;
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}