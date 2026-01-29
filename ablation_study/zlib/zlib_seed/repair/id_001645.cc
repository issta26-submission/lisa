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
//<ID> 1645
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateCopy -> deflateReset -> gzoffset64/gzeof usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream src_strm;
    z_stream copy_strm;
    memset(&src_strm, 0, (int)sizeof(src_strm));
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int init_ret = deflateInit_(&src_strm, 6, ver, (int)sizeof(z_stream));
    src_strm.next_in = (Bytef *)src;
    src_strm.avail_in = (uInt)src_len;
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *comp_buf2 = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    memset(comp_buf2, 0, (int)bound);
    src_strm.next_out = comp_buf;
    src_strm.avail_out = (uInt)bound;

    // step 2: Configure / Copy
    int copy_ret = deflateCopy(&copy_strm, &src_strm);
    copy_strm.next_out = comp_buf2;
    copy_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret_src = deflate(&src_strm, 4);
    uLong comp_len1 = src_strm.total_out;
    int def_ret_copy_first = deflate(&copy_strm, 4);
    uLong comp_len2 = copy_strm.total_out;
    int reset_ret = deflateReset(&copy_strm);
    copy_strm.next_in = (Bytef *)src;
    copy_strm.avail_in = (uInt)src_len;
    copy_strm.next_out = comp_buf2;
    copy_strm.avail_out = (uInt)bound;
    int def_ret_copy_second = deflate(&copy_strm, 4);
    uLong comp_len2_second = copy_strm.total_out;

    // step 4: Validate & Cleanup
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw1 = gzwrite(gzf, comp_buf, (unsigned int)comp_len1);
    int gw2 = gzwrite(gzf, comp_buf2, (unsigned int)comp_len2_second);
    off64_t offset = gzoffset64(gzf);
    int eof = gzeof(gzf);
    int def_end_src = deflateEnd(&src_strm);
    int def_end_copy = deflateEnd(&copy_strm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    free(comp_buf2);
    (void)ver;
    (void)init_ret;
    (void)copy_ret;
    (void)def_ret_src;
    (void)comp_len1;
    (void)def_ret_copy_first;
    (void)comp_len2;
    (void)reset_ret;
    (void)def_ret_copy_second;
    (void)comp_len2_second;
    (void)gw1;
    (void)gw2;
    (void)offset;
    (void)eof;
    (void)def_end_src;
    (void)def_end_copy;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}