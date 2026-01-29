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
//<ID> 1649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> deflateReset -> gzwrite -> gzeof/gzoffset64 usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret1 = deflate(&def_strm, 0);
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &def_strm);
    Bytef *copy_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(copy_buf, 0, (int)comp_bound);
    copy_strm.next_out = copy_buf;
    copy_strm.avail_out = (uInt)comp_bound;
    int reset_ret = deflateReset(&def_strm);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf + (uInt)def_strm.total_out;
    def_strm.avail_out = (uInt)(comp_bound - def_strm.total_out);
    int def_ret2 = deflate(&def_strm, 4);
    int def_ret3 = deflate(&copy_strm, 4);

    // step 4: Validate & Cleanup
    unsigned int out_len = (unsigned int)def_strm.total_out;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, out_len);
    off64_t off = gzoffset64(gzf);
    int eof_flag = gzeof(gzf);
    int gz_close_ret = gzclose(gzf);
    int def_end_ret1 = deflateEnd(&def_strm);
    int def_end_ret2 = deflateEnd(&copy_strm);
    free(comp_buf);
    free(copy_buf);
    (void)ver;
    (void)init_ret;
    (void)comp_bound;
    (void)def_ret1;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret2;
    (void)def_ret3;
    (void)gw_ret;
    (void)off;
    (void)eof_flag;
    (void)gz_close_ret;
    (void)def_end_ret1;
    (void)def_end_ret2;
    // API sequence test completed successfully
    return 66;
}