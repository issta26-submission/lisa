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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef src_buf[] = "Zlib API sequence payload for compress/write/read/getc/inflate.";
    uLong src_len = (uLong)(sizeof(src_buf) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = new Bytef[(size_t)bound];
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    gzFile gz_wr = (gzFile)0;
    gzFile gz_rd = (gzFile)0;
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    z_size_t read_items = 0;
    Bytef *read_buf = new Bytef[(size_t)bound];
    memset(read_buf, 0, (size_t)bound);
    int single_char = 0;
    int ret_compress = 0;
    int ret_gzwrite = 0;
    int ret_gzclose_wr = 0;
    int ret_gzclose_rd = 0;
    int ret_inflate_init = 0;
    int ret_inflate_end = 0;

    // step 2: Setup / Configure - compress source and write to .gz file
    ret_compress = compress2(comp_buf, &comp_len, src_buf, src_len, 6);
    gz_wr = gzopen(filename, "wb");
    ret_gzwrite = gzwrite(gz_wr, (voidpc)comp_buf, (unsigned int)comp_len);
    ret_gzclose_wr = gzclose(gz_wr);

    // step 3: Core operations - open for read, read with gzfread, get one char, initialize inflate
    gz_rd = gzopen(filename, "rb");
    read_items = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)comp_len, gz_rd);
    single_char = gzgetc(gz_rd);
    ret_inflate_init = inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));

    // step 4: Cleanup / Validate
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_gzclose_rd = gzclose(gz_rd);
    delete[] comp_buf;
    delete[] read_buf;
    (void)filename;
    (void)src_len;
    (void)bound;
    (void)comp_len;
    (void)read_items;
    (void)single_char;
    (void)ret_compress;
    (void)ret_gzwrite;
    (void)ret_gzclose_wr;
    (void)ret_gzclose_rd;
    (void)ret_inflate_init;
    (void)ret_inflate_end;
    // API sequence test completed successfully
    return 66;
}