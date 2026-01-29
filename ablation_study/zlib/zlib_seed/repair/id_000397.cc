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
//<ID> 397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef input[] = "Zlib API sequence payload for inflateInit_/inflateEnd/gzseek64/gzfread test.";
    uLong input_len = (uLong)(sizeof(input) - 1);
    Bytef comp_buf[1024];
    uLongf comp_len = (uLongf)sizeof(comp_buf);
    Bytef decomp_buf[1024];
    memset(&decomp_buf, 0, sizeof(decomp_buf));
    memset(&comp_buf, 0, sizeof(comp_buf));
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    const char *ver = zlibVersion();
    int compress_ret;
    int inflate_init_ret;
    int inflate_ret;
    int inflate_end_ret;
    gzFile gz_wr;
    gzFile gz_rd;
    off64_t seek_pos;
    z_size_t read_items;

    // step 2: Setup / Configure
    compress_ret = compress2(comp_buf, &comp_len, input, input_len, 6);
    gz_wr = gzopen64(filename, "wb");
    int gzwrite_ret = gzwrite(gz_wr, (voidpc)input, (unsigned int)input_len);
    int gzclose_wr_ret = gzclose(gz_wr);

    // step 3: Core operations
    inflate_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    infstrm.next_out = decomp_buf;
    infstrm.avail_out = (uInt)sizeof(decomp_buf);
    inflate_ret = inflate(&infstrm, 0);
    inflate_end_ret = inflateEnd(&infstrm);
    gz_rd = gzopen64(filename, "rb");
    seek_pos = gzseek64(gz_rd, (off64_t)0, 0);
    read_items = gzfread((voidp)decomp_buf, (z_size_t)1, (z_size_t)sizeof(decomp_buf), gz_rd);
    int gzclose_rd_ret = gzclose(gz_rd);

    // step 4: Validate / Cleanup
    (void)filename;
    (void)input_len;
    (void)compress_ret;
    (void)comp_len;
    (void)decomp_buf;
    (void)ver;
    (void)inflate_init_ret;
    (void)inflate_ret;
    (void)inflate_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_wr_ret;
    (void)seek_pos;
    (void)read_items;
    (void)gzclose_rd_ret;
    // API sequence test completed successfully
    return 66;
}