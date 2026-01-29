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
//<ID> 1794
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> reopen -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure and compress
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Write compressed data to a .gz file, flush and close; then reopen for read and get offset
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t wrote = gzfwrite(comp_buf, 1, (z_size_t)comp_len, gzf_w);
    off_t off_after_write = gzoffset(gzf_w);
    int gzflush_ret = gzflush(gzf_w, 2);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t off_on_read = gzoffset(gzf_r);
    int gzclose_r_ret = gzclose_r(gzf_r);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)wrote;
    (void)off_after_write;
    (void)gzflush_ret;
    (void)gzclose_w_ret;
    (void)off_on_read;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}