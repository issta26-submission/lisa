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
//<ID> 1796
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> uncompress -> validate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(comp_buf, 0, (size_t)bound);
    memset(decomp_buf, 0, (size_t)(src_len + 64));

    // step 2: Configure and compress
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate with gz I/O
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gzflush_ret = gzflush(gzf_w, 2);
    off_t offset_write = gzoffset(gzf_w);
    int gzclose_w_ret = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t offset_read = gzoffset(gzf_r);
    int gzclose_r_ret = gzclose_r(gzf_r);

    // step 4: Validate & Cleanup
    uLongf decomp_len_f = (uLongf)(src_len + 64);
    int un_ret = uncompress(decomp_buf, &decomp_len_f, comp_buf, comp_len);
    uLong checksum = adler32(0UL, decomp_buf, (uInt)decomp_len_f);
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)offset_write;
    (void)gzclose_w_ret;
    (void)offset_read;
    (void)gzclose_r_ret;
    (void)un_ret;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}