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
//<ID> 1646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> deflateReset -> gzoffset64/gzeof usage";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 2: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    z_stream dest_strm;
    memset(&dest_strm, 0, (int)sizeof(dest_strm));
    int copy_ret = deflateCopy(&dest_strm, &def_strm);
    int reset_ret = deflateReset(&def_strm);

    // step 3: Configure IO
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    off64_t offset64 = gzoffset64(gzf);
    int is_eof = gzeof(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int dest_end_ret = deflateEnd(&dest_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)reset_ret;
    (void)gw_ret;
    (void)offset64;
    (void)is_eof;
    (void)gzclose_ret;
    (void)dest_end_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}