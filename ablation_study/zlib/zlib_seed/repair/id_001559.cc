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
//<ID> 1559
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compress2 -> uncompress -> inflate stream check -> deflate cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong flags = zlibCompileFlags();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure
    z_stream inf_stream;
    z_stream def_stream;
    memset(&inf_stream, 0, (int)sizeof(inf_stream));
    memset(&def_stream, 0, (int)sizeof(def_stream));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_stream, 6, ver, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&inf_stream, ver, (int)sizeof(z_stream));
    Bytef *inflate_out = (Bytef *)malloc((size_t)src_len);
    memset(inflate_out, 0, (int)src_len);
    inf_stream.next_in = comp_buf;
    inf_stream.avail_in = (uInt)comp_len;
    inf_stream.next_out = inflate_out;
    inf_stream.avail_out = (uInt)src_len;

    // step 3: Operate
    Bytef *uncomp_out = (Bytef *)malloc((size_t)src_len);
    memset(uncomp_out, 0, (int)src_len);
    uLongf uncomp_len = src_len;
    int un_ret = uncompress(uncomp_out, &uncomp_len, comp_buf, comp_len);
    int inf_ret = inflate(&inf_stream, 0);
    unsigned long codes_used = inflateCodesUsed(&inf_stream);

    // step 4: Validate & Cleanup
    int inf_end_ret = inflateEnd(&inf_stream);
    int def_end_ret = deflateEnd(&def_stream);
    free(comp_buf);
    free(inflate_out);
    free(uncomp_out);
    (void)flags;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)un_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}