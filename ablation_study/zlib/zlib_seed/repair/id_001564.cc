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
//<ID> 1564
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateBound -> compressBound -> compress -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_stream;
    memset(&def_stream, 0, (int)sizeof(def_stream));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_stream, 6, ver, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&def_stream, src_len);
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress(comp_buf, &comp_len, src, src_len);

    // step 2: Configure
    z_stream inf_stream;
    memset(&inf_stream, 0, (int)sizeof(inf_stream));
    int inf_init_ret = inflateInit_(&inf_stream, ver, (int)sizeof(z_stream));
    inf_stream.next_in = comp_buf;
    inf_stream.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_stream.next_out = out_buf;
    inf_stream.avail_out = (uInt)src_len;
    z_stream def_copy;
    memset(&def_copy, 0, (int)sizeof(def_copy));
    int def_copy_ret = deflateCopy(&def_copy, &def_stream);

    // step 3: Operate
    int inf_ret = inflate(&inf_stream, 0);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_stream);
    int def_end_ret = deflateEnd(&def_stream);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)def_bound;
    (void)comp_bound;
    (void)comp_len;
    (void)comp_ret;
    (void)ver;
    (void)inf_init_ret;
    (void)def_copy_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}