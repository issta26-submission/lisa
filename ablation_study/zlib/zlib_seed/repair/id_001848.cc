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
//<ID> 1848
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz -> read gz -> compute crc -> init inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_before = crc32(0UL, src, (uInt)src_len);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gwritten = gzwrite(gw, src, (unsigned int)src_len);
    int gclose_w = gzclose(gw);
    (void)gwritten;
    (void)gclose_w;

    // step 2: Configure
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf1 = (Bytef *)malloc((size_t)(src_len + 8));
    Bytef *read_buf2 = (Bytef *)malloc((size_t)(src_len + 8));
    memset(read_buf1, 0, (int)(src_len + 8));
    memset(read_buf2, 0, (int)(src_len + 8));
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    const char *ver = zlibVersion();
    int inf_init_ret = inflateInit_(&inf, ver, (int)sizeof(z_stream));
    (void)inf_init_ret;

    // step 3: Operate
    z_size_t items_read1 = gzfread((voidp)read_buf1, (z_size_t)1, (z_size_t)src_len, gr);
    int rewind_ret = gzrewind(gr);
    z_size_t items_read2 = gzfread((voidp)read_buf2, (z_size_t)1, (z_size_t)src_len, gr);
    uLong crc_read1 = crc32(0UL, read_buf1, (uInt)(items_read1 > 0 ? (uInt)items_read1 : 0U));
    uLong crc_read2 = crc32(0UL, read_buf2, (uInt)(items_read2 > 0 ? (uInt)items_read2 : 0U));
    (void)rewind_ret;

    // step 4: Validate & Cleanup
    int inf_end = inflateEnd(&inf);
    int gclose_r = gzclose(gr);
    free(read_buf1);
    free(read_buf2);
    (void)ver;
    (void)crc_before;
    (void)crc_read1;
    (void)crc_read2;
    (void)inf_end;
    (void)gclose_r;
    // API sequence test completed successfully
    return 66;
}