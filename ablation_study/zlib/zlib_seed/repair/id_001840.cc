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
//<ID> 1840
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: write gz -> rewind -> read -> crc -> inflate init";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong crc_src = crc32(0UL, src, (uInt)src_len);
    const char *filename = "test_zlib_api_sequence_tmp.gz";

    // step 2: Write compressed data to a gz file
    gzFile gzf_w = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gzf_w, src, (unsigned int)(src_len > 0 ? (unsigned int)src_len : 0U));
    int gzclose_wret = gzclose(gzf_w);

    // step 3: Prepare inflate stream, rewind and read back the gz file
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int inflate_init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    gzFile gzf_r = gzopen(filename, "rb");
    int gzrewind_ret = gzrewind(gzf_r);
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    z_size_t nread = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)src_len, gzf_r);
    uLong crc_read = crc32(0UL, read_buf, (uInt)src_len);
    int gzclose_rret = gzclose(gzf_r);

    // step 4: Validate & Cleanup
    int inflate_end_ret = inflateEnd(&strm);
    free(read_buf);
    (void)crc_src;
    (void)crc_read;
    (void)gzwrite_ret;
    (void)gzclose_wret;
    (void)inflate_init_ret;
    (void)gzrewind_ret;
    (void)nread;
    (void)gzclose_rret;
    (void)inflate_end_ret;
    (void)ver;
    (void)filename;
    // API sequence test completed successfully
    return 66;
}