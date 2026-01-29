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
//<ID> 1847
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
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure & write gz file
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzf_w, src, (unsigned int)src_len);
    int gzclose_w_ret = gzclose(gzf_w);

    // step 3: Operate (open for read, rewind, and read contents)
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gzrewind_ret = gzrewind(gzf_r);
    Bytef read_buf[512];
    memset(read_buf, 0, (int)sizeof(read_buf));
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);

    // step 4: Validate & Cleanup
    uLong crc_read = crc32(0UL, read_buf, (uInt)items_read);
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)crc_src;
    (void)ver;
    (void)inf_init_ret;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzrewind_ret;
    (void)items_read;
    (void)gzclose_r_ret;
    (void)crc_read;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}