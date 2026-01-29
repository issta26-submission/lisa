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
//<ID> 1795
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> offsets -> close";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();

    // step 2: Configure deflate and allocate buffers
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate - deflate, write compressed data to gz file, flush and get offsets, then close
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t written = gzfwrite(comp_buf, 1, (z_size_t)comp_len, gzw);
    int flush_ret = gzflush(gzw, 2);
    off_t write_offset = gzoffset(gzw);
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t read_offset = gzoffset(gzr);
    int gzclose_r_ret = gzclose_r(gzr);

    // step 4: Validate & Cleanup
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)written;
    (void)flush_ret;
    (void)write_offset;
    (void)gzclose_w_ret;
    (void)read_offset;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}