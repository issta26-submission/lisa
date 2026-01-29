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
//<ID> 1799
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> read gz -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (write compressed data to a gz file, then read it back)
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t wrote = gzfwrite(comp_buf, 1, (z_size_t)comp_len, gzf_w);
    int flush_ret = gzflush(gzf_w, 0);
    off_t offset_after_write = gzoffset(gzf_w);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t offset_read_start = gzoffset(gzf_r);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 128));
    memset(decomp_buf, 0, (int)(src_len + 128));
    int gzread_ret = gzread(gzf_r, decomp_buf, (unsigned int)(src_len + 128));
    off_t offset_after_read = gzoffset(gzf_r);
    int gzclose_r_ret = gzclose_r(gzf_r);

    // step 4: Validate & Cleanup
    uLong check_adler = adler32(0UL, decomp_buf, (uInt)(gzread_ret > 0 ? (uInt)gzread_ret : 0U));
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)wrote;
    (void)flush_ret;
    (void)offset_after_write;
    (void)gzclose_w_ret;
    (void)offset_read_start;
    (void)gzread_ret;
    (void)offset_after_read;
    (void)gzclose_r_ret;
    (void)check_adler;
    // API sequence test completed successfully
    return 66;
}