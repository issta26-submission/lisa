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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef payload[] = "Zlib API sequence payload for gz write/read/getc and inflateEnd test.";
    uLong src_len = (uLong)(sizeof(payload) - 1);
    uLong bound = compressBound(src_len);
    Bytef *read_buf = (Bytef *)malloc((size_t)bound);
    z_size_t read_items;
    int getc_ret;
    gzFile gz_wr;
    gzFile gz_rd;
    z_stream inf_strm;
    int ret_inflate_init;
    int ret_inflate_end;
    int write_ret;
    int flush_ret;
    int close_wr_ret;
    int close_rd_ret;

    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, (size_t)bound);

    // step 2: Setup / write compressed file
    gz_wr = gzopen(filename, "wb");
    write_ret = gzwrite(gz_wr, (voidpc)payload, (unsigned int)src_len);
    flush_ret = gzflush(gz_wr, 0);
    close_wr_ret = gzclose(gz_wr);

    // step 3: Core operations - read and inspect
    gz_rd = gzopen(filename, "rb");
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)bound, gz_rd);
    getc_ret = gzgetc(gz_rd);
    close_rd_ret = gzclose(gz_rd);

    // step 4: Validate / Cleanup
    ret_inflate_init = inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_end = inflateEnd(&inf_strm);
    free(read_buf);
    (void)filename;
    (void)src_len;
    (void)bound;
    (void)read_items;
    (void)getc_ret;
    (void)write_ret;
    (void)flush_ret;
    (void)close_wr_ret;
    (void)close_rd_ret;
    (void)ret_inflate_init;
    (void)ret_inflate_end;
    // API sequence test completed successfully
    return 66;
}