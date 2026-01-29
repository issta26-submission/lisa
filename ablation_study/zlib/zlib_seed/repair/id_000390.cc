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
//<ID> 390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib payload for gz write/read used with gzseek64 and gzfread.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    int gzwrite_ret;
    int gzclose_wr_ret;
    int gzclose_rd_ret;
    z_size_t read_count;
    Bytef read_buf[512];
    z_stream inf_strm;
    const char *ver;
    int inflate_init_ret;
    int inflate_end_ret;
    off64_t seek_ret;

    // step 2: Setup / Configure
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    ver = zlibVersion();
    gz_wr = gzopen(filename, "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    gzclose_wr_ret = gzclose(gz_wr);

    // step 3: Core operations - open for read, seek with gzseek64, read with gzfread, init/inflateEnd
    gz_rd = gzopen(filename, "rb");
    seek_ret = gzseek64(gz_rd, (off64_t)0, 0);
    read_count = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)read_count;
    inflate_end_ret = inflateEnd(&inf_strm);
    gzclose_rd_ret = gzclose(gz_rd);

    // step 4: Validate / Cleanup
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzclose_wr_ret;
    (void)read_count;
    (void)seek_ret;
    (void)ver;
    (void)inflate_init_ret;
    (void)inflate_end_ret;
    (void)gzclose_rd_ret;
    // API sequence test completed successfully
    return 66;
}