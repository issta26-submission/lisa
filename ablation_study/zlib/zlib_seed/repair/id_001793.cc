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
//<ID> 1793
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate stream -> write gz -> read gz -> validate offsets";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure and compress
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Write compressed data to a gzip file, flush, get offset, close
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_size_t written = gzfwrite(comp_buf, 1, (z_size_t)comp_len, gzf);
    int gzflush_ret = gzflush(gzf, 2);
    off_t offset_after_write = gzoffset(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Read back the gz file, check offset, close (read-close), cleanup
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(read_buf, 0, (size_t)(src_len + 64));
    int read_ret = gzread(gzf_r, read_buf, (unsigned int)(src_len + 64));
    off_t offset_after_read = gzoffset(gzf_r);
    int gzclose_r_ret = gzclose_r(gzf_r);
    free(comp_buf);
    free(read_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)written;
    (void)gzflush_ret;
    (void)offset_after_write;
    (void)gzclose_ret;
    (void)read_ret;
    (void)offset_after_read;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}