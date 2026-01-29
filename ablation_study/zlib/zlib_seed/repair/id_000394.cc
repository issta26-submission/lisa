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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for inflateInit_/gzfread/gzseek64/inflateEnd test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    int gzwrite_ret;
    int gzclose_wr_ret;
    off64_t seek_ret;
    z_size_t read_items;
    Bytef read_buf[512];
    Bytef out_buf[1024];
    memset(read_buf, 0, sizeof(read_buf));
    memset(out_buf, 0, sizeof(out_buf));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    const char *ver = zlibVersion();
    int inflate_init_ret;
    int inflate_ret;
    int inflate_end_ret;
    int gzclose_rd_ret;

    // step 2: Setup / Configure
    inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 3: Core operations - write a gz file, then read and inflate from it
    gz_wr = gzopen(filename, "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    gzclose_wr_ret = gzclose(gz_wr);
    gz_rd = gzopen(filename, "rb");
    seek_ret = gzseek64(gz_rd, (off64_t)0, 0);
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)((read_items > 0) ? (uInt)read_items : 0);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)sizeof(out_buf);
    inflate_ret = inflate(&inf_strm, 0);
    gzclose_rd_ret = gzclose(gz_rd);

    // step 4: Validate / Cleanup
    inflate_end_ret = inflateEnd(&inf_strm);
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzclose_wr_ret;
    (void)seek_ret;
    (void)read_items;
    (void)inflate_init_ret;
    (void)inflate_ret;
    (void)gzclose_rd_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}