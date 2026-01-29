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
//<ID> 1537
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data, CRC, compression stream)
    const Bytef src[] = "zlib API sequence test: prepare, tune deflate, compress, write via gzfwrite, seek and crc32-check";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong src_crc = crc32(0UL, src, (uInt)src_len);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int tune_ret = deflateTune(&strm, 32, 16, 258, 16);

    // step 2: Configure (allocate compression buffer and perform compression)
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 3: Operate (open gzip file with gzopen64, write compressed data with gzfwrite, then seek)
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    z_size_t wrote = gzfwrite((voidpc)comp_buf, (z_size_t)1, (z_size_t)comp_len, gz);
    off_t seek_ret = gzseek(gz, (off_t)0, 0);
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_len);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&strm);
    int gz_close_ret = gzclose(gz);
    free(comp_buf);
    (void)src_crc;
    (void)def_init_ret;
    (void)tune_ret;
    (void)bound;
    (void)comp_ret;
    (void)wrote;
    (void)seek_ret;
    (void)comp_crc;
    (void)def_end_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}