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
//<ID> 1843
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz -> rewind -> read -> crc -> inflateInit_";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(infstrm));
    const char *ver = zlibVersion();
    int inflate_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));

    // step 2: Configure (write original data into a gz file)
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gz_written = gzwrite(gzf_w, (voidpc)src, (unsigned int)src_len);
    int gzclose_w_ret = gzclose(gzf_w);

    // step 3: Operate (open for read, rewind, and read using gzfread)
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    int gzrewind_ret = gzrewind(gzf);
    Bytef *read_buf = (Bytef *)malloc((size_t)src_len);
    memset(read_buf, 0, (int)src_len);
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)src_len, gzf);
    int gzclose_r_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    uLong crc_read = crc32(0UL, read_buf, (uInt)items_read);
    int inflate_end_ret = inflateEnd(&infstrm);
    free(read_buf);
    (void)ver;
    (void)inflate_init_ret;
    (void)gz_written;
    (void)gzclose_w_ret;
    (void)gzrewind_ret;
    (void)items_read;
    (void)gzclose_r_ret;
    (void)crc_src;
    (void)crc_read;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}