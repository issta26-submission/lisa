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
//<ID> 1797
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> offsets and close variants";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    const char *ver = zlibVersion();

    // step 2: Configure and compress using deflateInit_
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Write compressed output to a gzip file, flush and query offsets, then use read-close variant
    gzFile gzf_write = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzf_write, comp_buf, (unsigned int)comp_len);
    int gzflush_ret = gzflush(gzf_write, 0);
    off_t write_offset = gzoffset(gzf_write);
    int gzclose_ret = gzclose(gzf_write);
    gzFile gzf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    off_t read_offset = gzoffset(gzf_read);
    int gzclose_r_ret = gzclose_r(gzf_read);

    // step 4: Validate & Cleanup
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)write_offset;
    (void)gzclose_ret;
    (void)read_offset;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}