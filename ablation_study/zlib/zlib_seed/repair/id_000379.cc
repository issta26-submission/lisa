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
//<ID> 379
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gz write/read with inflateEnd and compressBound usage.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    int gzwrite_ret;
    int gzflush_ret;
    int gzclose_wr_ret;
    z_size_t read_items;
    Bytef read_buf[512];
    int gzgetc_ret;
    int gzclose_rd_ret;
    uLong bound;
    unsigned char comp_buf[1024];
    z_stream inf_strm;
    const char *ver;
    int inflate_init_ret;
    int inflate_end_ret;

    // step 2: Setup / Configure
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    ver = zlibVersion();
    bound = compressBound((uLong)write_len);
    comp_buf[0] = (unsigned char)(bound & 0xFF);

    // step 3: Core operations - write compressed file, read it back, initialize inflate, cleanup inflate
    gz_wr = gzopen(filename, "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidp)write_buf, write_len);
    gzflush_ret = gzflush(gz_wr, 0);
    gzclose_wr_ret = gzclose(gz_wr);
    inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    gz_rd = gzopen(filename, "rb");
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    gzgetc_ret = gzgetc(gz_rd);
    gzclose_rd_ret = gzclose(gz_rd);
    inflate_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate / Cleanup
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_wr_ret;
    (void)read_items;
    (void)gzgetc_ret;
    (void)gzclose_rd_ret;
    (void)bound;
    (void)comp_buf;
    (void)ver;
    (void)inflate_init_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}