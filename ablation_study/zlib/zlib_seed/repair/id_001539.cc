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
//<ID> 1539
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflateTune, compress2, write gz with gzopen64/gzfwrite and crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong orig_crc = crc32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&strm, 4, 16, 32, 128);

    // step 2: Configure
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t written = gzfwrite((voidpc)comp_buf, (z_size_t)1, (z_size_t)comp_len, gz);
    off_t pos_after_write = gzseek(gz, (off_t)0, 0);

    // step 3: Operate
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_len);
    off_t pos_verify = gzseek(gz, pos_after_write, 0);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    (void)orig_crc;
    (void)def_init_ret;
    (void)tune_ret;
    (void)bound;
    (void)comp_ret;
    (void)written;
    (void)pos_after_write;
    (void)comp_crc;
    (void)pos_verify;
    (void)def_end_ret;
    (void)gz_close_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}