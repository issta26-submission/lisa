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
//<ID> 391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API payload for gzwrite/read + inflateInit_/inflateEnd test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    int gzwrite_ret;
    int gzclose_wr_ret;
    z_stream inf_strm;
    const char *ver;
    z_size_t read_items;
    Bytef read_buf[512];
    off64_t seek_ret;
    int inflate_init_ret;
    int inflate_end_ret;
    int gzclose_rd_ret;

    // step 2: Setup / Write compressed file
    memset(&inf_strm, 0, (size_t)sizeof(inf_strm));
    memset(read_buf, 0, (size_t)sizeof(read_buf));
    ver = zlibVersion();
    gz_wr = gzopen(filename, "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidp)write_buf, write_len);
    gzclose_wr_ret = gzclose(gz_wr);

    // step 3: Read compressed file, seek, initialize inflate, attach read buffer
    gz_rd = gzopen64(filename, "rb");
    seek_ret = gzseek64(gz_rd, (off64_t)0, 0);
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)((read_items > (z_size_t)0xFFFFFFFFu) ? 0xFFFFFFFFu : (uInt)read_items);

    // step 4: Validate / Cleanup
    gzclose_rd_ret = gzclose(gz_rd);
    inflate_end_ret = inflateEnd(&inf_strm);
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzclose_wr_ret;
    (void)ver;
    (void)read_items;
    (void)seek_ret;
    (void)inflate_init_ret;
    (void)gzclose_rd_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}