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
//<ID> 1715
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: checksum -> deflateParams -> deflate -> gzoffset64/gzdirect";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    uLong checksum1 = adler32(0L, src, (uInt)src_len);
    strm.adler = checksum1;

    // step 2: Configure
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int params_ret = deflateParams(&strm, 9, 0);
    int reset_ret = deflateReset(&strm);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    int end_ret = deflateEnd(&strm);
    gzFile gz_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gz_w, comp_buf, (unsigned int)comp_len);
    off64_t offset64 = gzoffset64(gz_w);
    int direct_flag = gzdirect(gz_w);
    int gzclose_w = gzclose(gz_w);

    // step 4: Validate & Cleanup
    gzFile gz_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    unsigned char read_buf[256];
    memset(read_buf, 0, 256);
    int read_len = gzread(gz_r, read_buf, (unsigned int)(sizeof(read_buf) - 1));
    uLong checksum2 = adler32(0L, read_buf, (uInt)(read_len > 0 ? read_len : 0));
    int gzclose_r = gzclose(gz_r);
    free(comp_buf);
    (void)init_ret;
    (void)checksum1;
    (void)bound;
    (void)params_ret;
    (void)reset_ret;
    (void)def_ret;
    (void)comp_len;
    (void)end_ret;
    (void)gw_ret;
    (void)offset64;
    (void)direct_flag;
    (void)gzclose_w;
    (void)read_len;
    (void)checksum2;
    (void)gzclose_r;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}