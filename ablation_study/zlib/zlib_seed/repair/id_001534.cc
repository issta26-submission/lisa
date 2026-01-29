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
//<ID> 1534
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: compute crc32, compress, tune deflate, write gzip and seek";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress((Bytef *)comp_buf, &comp_len, (const Bytef *)src, src_len);
    uLong src_crc = crc32(0UL, src, (uInt)src_len);

    // step 2: Configure
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int def_tune_ret = deflateTune(&strm, 32, 64, 64, 1024);

    // step 3: Operate
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t wrote = gzfwrite((voidpc)comp_buf, (z_size_t)1, (z_size_t)comp_len, gz);
    off_t pos = gzseek(gz, (off_t)0, 0);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    (void)comp_ret;
    (void)src_crc;
    (void)def_init_ret;
    (void)def_tune_ret;
    (void)ver;
    (void)wrote;
    (void)pos;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}