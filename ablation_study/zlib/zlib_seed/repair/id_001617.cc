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
//<ID> 1617
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compressBound + deflate + gzwrite + gzclearerr + zError";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure & Operate
    uLong alloc_size = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)alloc_size);
    memset(comp_buf, 0, (int)alloc_size);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)alloc_size;
    int def_ret = deflate(&def_strm, 4);
    uLong produced = def_strm.total_out;

    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)produced);
    gzclearerr(gzf);

    // step 3: Validate
    const char *err_msg = zError(def_ret);
    (void)err_msg;

    // step 4: Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);

    (void)ver;
    (void)init_ret;
    (void)alloc_size;
    (void)produced;
    (void)gw_ret;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}