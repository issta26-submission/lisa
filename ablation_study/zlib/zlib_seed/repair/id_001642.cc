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
//<ID> 1642
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
    Bytef *comp_buf1 = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf1, 0, (int)comp_bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf2, 0, (int)comp_bound);

    z_stream src_strm;
    memset(&src_strm, 0, (int)sizeof(src_strm));
    int init_ret = deflateInit_(&src_strm, 6, ver, (int)sizeof(z_stream));
    src_strm.next_in = (Bytef *)src;
    src_strm.avail_in = (uInt)src_len;
    src_strm.next_out = comp_buf1;
    src_strm.avail_out = (uInt)comp_bound;

    // step 2: Configure
    int def_ret1 = deflate(&src_strm, 4);
    uLong comp_len1 = src_strm.total_out;
    z_stream dest_strm;
    memset(&dest_strm, 0, (int)sizeof(dest_strm));
    int copy_ret = deflateCopy(&dest_strm, &src_strm);
    int reset_ret = deflateReset(&dest_strm);
    dest_strm.next_in = (Bytef *)src;
    dest_strm.avail_in = (uInt)src_len;
    dest_strm.next_out = comp_buf2;
    dest_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret2 = deflate(&dest_strm, 4);
    uLong comp_len2 = dest_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    off64_t offset = gzoffset64(gzf);
    int eof_flag = gzeof(gzf);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int end_ret_src = deflateEnd(&src_strm);
    int end_ret_dest = deflateEnd(&dest_strm);
    free(comp_buf1);
    free(comp_buf2);
    (void)ver;
    (void)comp_bound;
    (void)init_ret;
    (void)def_ret1;
    (void)comp_len1;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret2;
    (void)comp_len2;
    (void)gw_ret;
    (void)offset;
    (void)eof_flag;
    (void)gz_close_ret;
    (void)end_ret_src;
    (void)end_ret_dest;
    // API sequence test completed successfully
    return 66;
}