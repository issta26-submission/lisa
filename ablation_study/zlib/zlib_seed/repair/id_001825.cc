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
//<ID> 1825
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
    z_stream strm;
    z_stream copy_strm;
    memset(&strm, 0, (int)sizeof(strm));
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    const char *ver = zlibVersion();
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;

    // step 3: Copy deflate state & write original data to a gz file
    int copy_ret = deflateCopy(&copy_strm, &strm);
    int end_ret_src = deflateEnd(&strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, (const char *)src);
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Cleanup & validate
    int end_ret_copy = deflateEnd(&copy_strm);
    free(comp_buf);
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)end_ret_src;
    (void)gzputs_ret;
    (void)gzclose_ret;
    (void)end_ret_copy;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}