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
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API payload: write then read with gzfread, gzseek64, inflateInit_ and inflateEnd.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    int gzwrite_ret;
    int gzflush_ret;
    int gzclose_wr_ret;
    z_size_t read_items;
    Bytef comp_buf[1024];
    Bytef out_buf[2048];
    off64_t seek_ret;
    z_stream inf_strm;
    const char *ver;
    int inflate_init_ret;
    int inflate_end_ret;
    int gzclose_rd_ret;

    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(out_buf, 0, sizeof(out_buf));

    // step 2: Setup / Write compressed file
    gz_wr = gzopen(filename, "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidp)write_buf, write_len);
    gzflush_ret = gzflush(gz_wr, 0);
    gzclose_wr_ret = gzclose(gz_wr);

    // step 3: Operate / Read, seek, initialize inflate with data pointers
    gz_rd = gzopen(filename, "rb");
    read_items = gzfread((voidp)comp_buf, (z_size_t)1, (z_size_t)sizeof(comp_buf), gz_rd);
    seek_ret = gzseek64(gz_rd, (off64_t)0, 0);
    ver = zlibVersion();
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)read_items;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)sizeof(out_buf);
    inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 4: Validate / Cleanup
    inflate_end_ret = inflateEnd(&inf_strm);
    gzclose_rd_ret = gzclose(gz_rd);
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_wr_ret;
    (void)read_items;
    (void)seek_ret;
    (void)ver;
    (void)inflate_init_ret;
    (void)inflate_end_ret;
    (void)gzclose_rd_ret;
    (void)comp_buf;
    (void)out_buf;
    // API sequence test completed successfully
    return 66;
}