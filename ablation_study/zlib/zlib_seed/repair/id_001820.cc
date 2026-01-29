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
//<ID> 1820
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib api sequence: deflate -> deflateCopy -> write gz -> clearerr";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    z_stream copy_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Operate (copy stream state, write gzip content, clear gz error state)
    int def_copy_ret = deflateCopy(&copy_strm, &def_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret1 = gzputs(gzf, "zlib sequence output header\n");
    int gzputs_ret2 = gzputs(gzf, (const char *)src);
    gzclearerr(gzf);

    // step 4: Cleanup & Validate
    int def_end_ret_orig = deflateEnd(&def_strm);
    int def_end_ret_copy = deflateEnd(&copy_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_copy_ret;
    (void)gzputs_ret1;
    (void)gzputs_ret2;
    (void)def_end_ret_orig;
    (void)def_end_ret_copy;
    (void)gzclose_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}