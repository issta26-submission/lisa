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
//<ID> 1540
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz, read back, compute crc32, and exercise inflateUndermine";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong orig_crc = crc32(0UL, src, (uInt)src_len);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len);
    int gz_write_ret = gzwrite(gzw, (voidpc)src, write_len);
    int gz_close_w_ret = gzclose(gzw);

    // step 2: Configure
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    int direct_flag = gzdirect(gzr);

    // step 3: Operate
    uLong read_alloc = src_len + 64UL;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_alloc);
    memset(read_buf, 0, (int)read_alloc);
    int gz_read_ret = gzread(gzr, (voidp)read_buf, (unsigned int)read_alloc);
    uLong read_crc = crc32(0UL, read_buf, (uInt)gz_read_ret);

    // step 4: Validate & Cleanup
    z_stream strm;
    memset(&strm, 0, (int)sizeof(z_stream));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    int und_ret = inflateUndermine(&strm, 1);
    int inf_end_ret = inflateEnd(&strm);
    int gz_close_r_ret = gzclose(gzr);
    free(read_buf);
    (void)orig_crc;
    (void)gz_write_ret;
    (void)gz_close_w_ret;
    (void)direct_flag;
    (void)gz_read_ret;
    (void)read_crc;
    (void)inf_init_ret;
    (void)und_ret;
    (void)inf_end_ret;
    (void)gz_close_r_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}