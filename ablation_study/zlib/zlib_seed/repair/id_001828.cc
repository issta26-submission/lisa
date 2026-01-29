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
//<ID> 1828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> gzopen/gzputs -> validate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm_src;
    z_stream strm_copy;
    memset(&strm_src, 0, (int)sizeof(strm_src));
    memset(&strm_copy, 0, (int)sizeof(strm_copy));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm_src, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    strm_src.next_in = (Bytef *)src;
    strm_src.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm_src, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm_src.next_out = comp_buf;
    strm_src.avail_out = (uInt)bound;
    int def_ret = deflate(&strm_src, 4);
    uLong comp_len = strm_src.total_out;

    // step 3: Copy deflate state, write to gz and clear errors
    int copy_ret = deflateCopy(&strm_copy, &strm_src);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, (const char *)src);
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong check_adler = adler32(0UL, src, (uInt)src_len);
    int end_src_ret = deflateEnd(&strm_src);
    int end_copy_ret = deflateEnd(&strm_copy);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)gzputs_ret;
    (void)gzclose_ret;
    (void)check_adler;
    (void)end_src_ret;
    (void)end_copy_ret;
    // API sequence test completed successfully
    return 66;
}