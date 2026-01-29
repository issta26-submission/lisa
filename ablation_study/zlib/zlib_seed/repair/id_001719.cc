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
//<ID> 1719
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib sequence: compute adler32 -> deflateInit_ -> deflateParams -> deflateReset -> deflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong checksum = adler32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int params_ret = deflateParams(&strm, 6, 0);
    int reset_ret = deflateReset(&strm);

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 4: Validate & Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf, src, (unsigned int)src_len);
    off64_t offset_after_write = gzoffset64(gzf);
    int direct_flag = gzdirect(gzf);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    (void)ver;
    (void)checksum;
    (void)init_ret;
    (void)params_ret;
    (void)reset_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)offset_after_write;
    (void)direct_flag;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}