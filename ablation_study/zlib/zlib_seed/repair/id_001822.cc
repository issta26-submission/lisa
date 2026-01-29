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
//<ID> 1822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate init -> deflate -> deflateCopy -> gz write -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream src_strm;
    z_stream copy_strm;
    memset(&src_strm, 0, (int)sizeof(src_strm));
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&src_strm, 6, ver, (int)sizeof(z_stream));
    src_strm.next_in = (Bytef *)src;
    src_strm.avail_in = (uInt)src_len;

    // step 2: Configure & Compress
    uLong bound = deflateBound(&src_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    src_strm.next_out = comp_buf;
    src_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&src_strm, 4);
    uLong comp_len = src_strm.total_out;
    int defcopy_ret = deflateCopy(&copy_strm, &src_strm);

    // step 3: Operate (write to gz) and clear errors
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, "zlib sequence: compressed data chunk");
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int def_end_ret_src = deflateEnd(&src_strm);
    int def_end_ret_copy = deflateEnd(&copy_strm);
    uLong check_adler = adler32(0UL, src, (uInt)src_len);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)defcopy_ret;
    (void)gzputs_ret;
    (void)gzclose_ret;
    (void)def_end_ret_src;
    (void)def_end_ret_copy;
    (void)check_adler;
    // API sequence test completed successfully
    return 66;
}