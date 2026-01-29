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
//<ID> 1563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateBound -> deflate -> deflateCopy -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong cbound = compressBound(src_len);

    // step 2: Configure
    z_stream def_stream;
    z_stream def_copy;
    z_stream inf_stream;
    memset(&def_stream, 0, (int)sizeof(def_stream));
    memset(&def_copy, 0, (int)sizeof(def_copy));
    memset(&inf_stream, 0, (int)sizeof(inf_stream));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_stream, 6, ver, (int)sizeof(z_stream));
    uLong dbound = deflateBound(&def_stream, src_len);
    uLong comp_alloc = dbound + cbound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_alloc);
    memset(comp_buf, 0, (int)comp_alloc);
    def_stream.next_in = (Bytef *)src;
    def_stream.avail_in = (uInt)src_len;
    def_stream.next_out = comp_buf;
    def_stream.avail_out = (uInt)comp_alloc;

    // step 3: Operate
    int def_ret = deflate(&def_stream, 4); // 4 == Z_FINISH
    int copy_ret = deflateCopy(&def_copy, &def_stream);
    int inf_init_ret = inflateInit_(&inf_stream, ver, (int)sizeof(z_stream));
    inf_stream.next_in = comp_buf;
    inf_stream.avail_in = (uInt)(def_stream.total_out);
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_stream.next_out = out_buf;
    inf_stream.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_stream, 0);

    // step 4: Validate & Cleanup
    uLong recovered = inf_stream.total_out;
    int def_end_ret = deflateEnd(&def_stream);
    int def_copy_end_ret = deflateEnd(&def_copy);
    int inf_end_ret = inflateEnd(&inf_stream);
    free(comp_buf);
    free(out_buf);
    (void)cbound;
    (void)dbound;
    (void)comp_alloc;
    (void)def_init_ret;
    (void)def_ret;
    (void)copy_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)recovered;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}