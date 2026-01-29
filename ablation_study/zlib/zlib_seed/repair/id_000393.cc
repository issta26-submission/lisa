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
//<ID> 393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    const char *ver = zlibVersion();
    Bytef write_buf[] = "Zlib API sequence payload for gzwrite/gzseek64/gzfread with inflateInit_ and inflateEnd.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    gzFile gz_rd;
    int gzwrite_ret;
    z_size_t read_items;
    Bytef read_buf[512];
    off64_t seek_ret;
    z_stream inf_strm;
    int inflate_init_ret;
    int inflate_end_ret;

    // step 2: Setup / Configure
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    gz_wr = gzopen64(filename, "wb");
    gzwrite_ret = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    gzclose(gz_wr);
    inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 3: Core operations - open, seek, read, attach to inflate stream
    gz_rd = gzopen64(filename, "rb");
    seek_ret = gzseek64(gz_rd, (off64_t)0, 0);
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz_rd);
    gzclose(gz_rd);
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)read_items;
    inf_strm.total_in = 0;

    // step 4: Validate / Cleanup
    inflate_end_ret = inflateEnd(&inf_strm);
    (void)filename;
    (void)ver;
    (void)write_len;
    (void)gzwrite_ret;
    (void)seek_ret;
    (void)read_items;
    (void)inflate_init_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}