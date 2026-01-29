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
//<ID> 1569
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateBound -> deflateCopy -> deflate -> inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_out, 0, (int)def_bound);
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)def_bound;

    // step 2: Configure
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &def_strm);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    memcpy(comp_buf, def_out, (size_t)comp_len);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)src_len;
    int inf_ret = inflate(&inf_strm, 0);

    // step 4: Validate & Cleanup
    int inf_end = inflateEnd(&inf_strm);
    int def_end = deflateEnd(&def_strm);
    int copy_end = deflateEnd(&copy_strm);
    free(comp_buf);
    free(def_out);
    free(out_buf);

    (void)ver;
    (void)comp_bound;
    (void)def_bound;
    (void)def_init_ret;
    (void)copy_ret;
    (void)def_ret;
    (void)comp_len;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end;
    (void)def_end;
    (void)copy_end;
    // API sequence test completed successfully
    return 66;
}