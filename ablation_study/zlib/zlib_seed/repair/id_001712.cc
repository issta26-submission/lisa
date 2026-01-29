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
//<ID> 1712
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateInit_ -> deflateParams -> deflate -> gzwrite -> gzoffset64";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int params_ret = deflateParams(&strm, 9, 0);

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    int reset_ret = deflateReset(&strm);

    // step 4: Validate & Cleanup
    uLong checksum = adler32(0UL, src, (uInt)src_len);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int direct_flag = gzdirect(gzf);
    off64_t offset64 = gzoffset64(gzf);
    int gzclose_ret = gzclose(gzf);
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)init_ret;
    (void)params_ret;
    (void)def_ret;
    (void)reset_ret;
    (void)checksum;
    (void)gw_ret;
    (void)direct_flag;
    (void)offset64;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}