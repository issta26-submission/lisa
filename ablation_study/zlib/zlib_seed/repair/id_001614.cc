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
//<ID> 1614
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compressBound + deflate + gzclearerr + zError usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_buf_size = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_buf_size);
    memset(comp_buf, 0, (int)comp_buf_size);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)comp_buf_size;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    gzclearerr(gzf);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&strm);
    const char *err_str = zError(def_ret);
    free(comp_buf);

    (void)ver;
    (void)def_init_ret;
    (void)comp_len;
    (void)gw_ret;
    (void)gz_close_ret;
    (void)def_end_ret;
    (void)err_str;
    // API sequence test completed successfully
    return 66;
}