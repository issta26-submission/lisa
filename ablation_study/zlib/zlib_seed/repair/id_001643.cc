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
//<ID> 1643
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> deflateReset -> gzoffset64/gzeof usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 2: Configure
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    z_stream def_copy;
    memset(&def_copy, 0, (int)sizeof(def_copy));
    int copy_ret = deflateCopy(&def_copy, &def_strm);
    int reset_ret = deflateReset(&def_strm);

    // step 3: Operate
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off64_t offset = gzoffset64(gzf);
    int eof_ret = gzeof(gzf);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int end_ret = deflateEnd(&def_strm);
    int end_copy_ret = deflateEnd(&def_copy);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)reset_ret;
    (void)gw_ret;
    (void)offset;
    (void)eof_ret;
    (void)gz_close_ret;
    (void)end_ret;
    (void)end_copy_ret;
    return 66;
    // API sequence test completed successfully
}