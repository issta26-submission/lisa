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
//<ID> 1826
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> gzputs -> gzclearerr";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    z_stream def_copy;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&def_copy, 0, (int)sizeof(def_copy));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Copy deflate state, write to gz and clear errors
    memset(&def_copy, 0, (int)sizeof(def_copy));
    int def_copy_ret = deflateCopy(&def_copy, &def_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, "gzputs: writing a test line from zlib API sequence");
    gzclearerr(gzf);

    // step 4: Cleanup & Validate
    int def_end_ret_src = deflateEnd(&def_strm);
    int def_end_ret_copy = deflateEnd(&def_copy);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_copy_ret;
    (void)gzputs_ret;
    (void)def_end_ret_src;
    (void)def_end_ret_copy;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}