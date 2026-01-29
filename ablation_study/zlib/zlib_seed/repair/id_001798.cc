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
//<ID> 1798
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflate -> gzwrite/gzread -> checksums";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong comp_bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4); // 4 == Z_FINISH
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_len);
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, (voidp)src, (unsigned int)src_len);
    int gzflush_ret = gzflush(gzw, 2);
    off_t write_offset = gzoffset(gzw);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(read_buf, 0, (int)(src_len + 64));
    int gzread_ret = gzread(gzr, (voidp)read_buf, (unsigned int)(src_len + 64));
    off_t read_offset = gzoffset(gzr);
    int gzclose_r_ret = gzclose_r(gzr);

    // step 4: Validate & Cleanup
    uLong read_adler = adler32(0UL, read_buf, (uInt)gzread_ret);
    free(comp_buf);
    free(read_buf);
    (void)ver;
    (void)def_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)comp_crc;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)write_offset;
    (void)gzclose_w_ret;
    (void)gzread_ret;
    (void)read_offset;
    (void)gzclose_r_ret;
    (void)read_adler;
    // API sequence test completed successfully
    return 66;
}