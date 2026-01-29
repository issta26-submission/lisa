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
//<ID> 1640
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
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &def_strm);
    int reset_ret = deflateReset(&copy_strm);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf2, 0, (int)comp_bound);
    copy_strm.next_in = (Bytef *)src;
    copy_strm.avail_in = (uInt)src_len;
    copy_strm.next_out = comp_buf2;
    copy_strm.avail_out = (uInt)comp_bound;
    int def_copy_ret = deflate(&copy_strm, 4);
    uLong out_len_orig = def_strm.total_out;
    uLong out_len_copy = copy_strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gzf_eof = gzeof(gzf);
    off64_t offset = gzoffset64(gzf);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int copy_end_ret = deflateEnd(&copy_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(comp_buf2);
    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_copy_ret;
    (void)out_len_orig;
    (void)out_len_copy;
    (void)gw_ret;
    (void)gzf_eof;
    (void)offset;
    (void)def_end_ret;
    (void)copy_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}