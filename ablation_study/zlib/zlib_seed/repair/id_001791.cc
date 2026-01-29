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
//<ID> 1791
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> write gz -> offset -> close";
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
    int def_ret = deflate(&def_strm, 4); // Z_FINISH == 4
    uLong comp_len = def_strm.total_out;

    // step 3: Operate with gzFile: write compressed data, flush and get offset, close write handle
    gzFile gzf_write = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t written = gzfwrite(comp_buf, 1, (z_size_t)comp_len, gzf_write);
    int flush_ret = gzflush(gzf_write, 2); // Z_SYNC_FLUSH == 2 (use numeric value)
    off_t offset_after_write = gzoffset(gzf_write);
    int gzclose_write_ret = gzclose(gzf_write);

    // step 4: Validate & Cleanup
    gzFile gzf_read = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gzclose_read_ret = gzclose_r(gzf_read);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)written;
    (void)flush_ret;
    (void)offset_after_write;
    (void)gzclose_write_ret;
    (void)gzclose_read_ret;
    (void)def_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}