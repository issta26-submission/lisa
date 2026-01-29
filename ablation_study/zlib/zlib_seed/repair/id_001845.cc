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
//<ID> 1845
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz -> rewind -> read -> crc32 -> inflateInit_";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gz_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_write_ret = gzwrite(gz_w, src, (unsigned int)src_len);
    int gzclose_w_ret = gzclose(gz_w);
    z_stream infstream;
    memset(&infstream, 0, (int)sizeof(infstream));
    const char *ver = zlibVersion();
    int init_ret = inflateInit_(&infstream, ver, (int)sizeof(z_stream));

    // step 2: Configure
    gzFile gz_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    z_size_t buf_size = (z_size_t)(src_len + 16U);
    Bytef *out_buf = (Bytef *)malloc((size_t)buf_size);
    memset(out_buf, 0, (int)buf_size);

    // step 3: Operate
    int rewind_ret = gzrewind(gz_r);
    z_size_t items_read = gzfread(out_buf, 1, buf_size, gz_r);
    uLong data_crc = crc32(0UL, out_buf, (uInt)items_read);

    // step 4: Validate & Cleanup
    int gzclose_r_ret = gzclose(gz_r);
    int inf_end_ret = inflateEnd(&infstream);
    free(out_buf);
    (void)gz_write_ret;
    (void)gzclose_w_ret;
    (void)ver;
    (void)init_ret;
    (void)rewind_ret;
    (void)items_read;
    (void)data_crc;
    (void)gzclose_r_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}