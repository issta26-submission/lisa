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
//<ID> 378
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef src[] = "Zlib API sequence payload for testing compressBound, gz I/O and inflateEnd.";
    uLong srcLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(srcLen);
    uLongf compDestLen = (uLongf)bound;
    Bytef *compDest = new Bytef[compDestLen];
    int comp_ret;
    gzFile gz_wr;
    gzFile gz_rd;
    int gzwrite_ret;
    int gzflush_ret;
    int gzclose_wr_ret;
    z_size_t read_items;
    Bytef read_buf[128];
    int getc_ret;
    int gzclose_rd_ret;
    z_stream inf_strm;
    const char *ver = zlibVersion();
    int inflate_init_ret;
    int inflate_end_ret;

    // step 2: Setup / Compress
    comp_ret = compress2(compDest, &compDestLen, src, srcLen, 6);

    // step 3: Operate - write compressed stream with gz, then read and inspect
    gz_wr = gzopen(filename, "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidpc)src, (unsigned int)srcLen);
    gzflush_ret = gzflush(gz_wr, 0);
    gzclose_wr_ret = gzclose(gz_wr);
    gz_rd = gzopen(filename, "rb");
    memset(read_buf, 0, sizeof(read_buf));
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    getc_ret = gzgetc(gz_rd);
    gzclose_rd_ret = gzclose_r(gz_rd);

    // step 4: Validate / Cleanup
    memset(&inf_strm, 0, sizeof(inf_strm));
    inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inflate_end_ret = inflateEnd(&inf_strm);
    delete [] compDest;
    (void)filename;
    (void)srcLen;
    (void)bound;
    (void)compDestLen;
    (void)comp_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_wr_ret;
    (void)read_items;
    (void)read_buf;
    (void)getc_ret;
    (void)gzclose_rd_ret;
    (void)inflate_init_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}