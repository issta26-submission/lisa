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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gzwrite/gzfread/gzgetc and inflateEnd usage.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    z_size_t read_items;
    Bytef read_buf[256];
    int getc_ret;
    z_stream inf_strm;
    int inflate_init_ret;
    int inflate_ret;
    int gzwrite_ret;
    int gzbuffer_ret;
    int gzclose_wr_ret;
    int gzclose_rd_ret;
    uLong bound;

    // step 2: Setup / write compressed file
    memset(&inf_strm, 0, sizeof(inf_strm));
    gz_wr = gzopen(filename, "wb");
    bound = compressBound((uLong)write_len);
    gzbuffer_ret = gzbuffer(gz_wr, (unsigned int)bound);
    gzwrite_ret = gzwrite(gz_wr, write_buf, write_len);
    gzclose_wr_ret = gzclose(gz_wr);

    // step 3: Read back and attempt inflate then cleanup inflate state
    gz_rd = gzopen(filename, "rb");
    memset(read_buf, 0, sizeof(read_buf));
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    getc_ret = gzgetc(gz_rd);
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)read_items;
    inf_strm.next_out = read_buf;
    inf_strm.avail_out = (uInt)sizeof(read_buf);
    inflate_init_ret = inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inflate_ret = inflate(&inf_strm, 0);
    inflateEnd(&inf_strm);
    gzclose_rd_ret = gzclose_r(gz_rd);

    // step 4: Validate / final cleanup
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzbuffer_ret;
    (void)gzclose_wr_ret;
    (void)read_items;
    (void)getc_ret;
    (void)inflate_init_ret;
    (void)inflate_ret;
    (void)gzclose_rd_ret;
    (void)bound;
    (void)read_buf;
    // API sequence test completed successfully
    return 66;
}