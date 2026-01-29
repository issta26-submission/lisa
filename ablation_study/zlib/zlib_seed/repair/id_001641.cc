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
//<ID> 1641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateCopy -> deflateReset -> gzwrite usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream orig_strm;
    memset(&orig_strm, 0, (int)sizeof(orig_strm));
    int def_init_ret = deflateInit_(&orig_strm, 6, ver, (int)sizeof(z_stream));
    orig_strm.next_in = (Bytef *)src;
    orig_strm.avail_in = (uInt)src_len;

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    orig_strm.next_out = comp_buf;
    orig_strm.avail_out = (uInt)comp_bound;
    z_stream copy_strm;
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int copy_ret = deflateCopy(&copy_strm, &orig_strm);
    int reset_ret = deflateReset(&copy_strm);

    // step 3: Operate
    int def_ret = deflate(&orig_strm, 4);
    uLong comp_len = orig_strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int write_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off64_t off = gzoffset64(gzf);
    int eof_ret = gzeof(gzf);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&orig_strm);
    int copy_end_ret = deflateEnd(&copy_strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)comp_bound;
    (void)copy_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)write_ret;
    (void)off;
    (void)eof_ret;
    (void)gz_close_ret;
    (void)def_end_ret;
    (void)copy_end_ret;
    // API sequence test completed successfully
    return 66;
}