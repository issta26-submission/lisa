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
//<ID> 1045
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: deflate into a buffer, write via gz, then read back with gzfread.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure / Operate (deflate)
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, Z_FINISH);
    unsigned int comp_size = (unsigned int)strm.total_out;
    int def_end_ret = deflateEnd(&strm);

    // step 3: Write compressed data to a gz file and append text
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, comp_size);
    int gzputs_ret = gzputs(gzw, "APPENDED_LINE\n");
    int gzflush_ret = gzflush(gzw, 2);
    int gzclose_ret_w = gzclose(gzw);

    // step 4: Read back, use gzfread and cleanup
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    uLong read_buf_size = source_len + 256;
    voidp read_buf = malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)read_buf_size, gzr);
    int gzclose_ret_r = gzclose(gzr);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzputs_ret;
    (void)gzflush_ret;
    (void)gzclose_ret_w;
    (void)items_read;
    (void)gzclose_ret_r;
    // API sequence test completed successfully
    return 66;
}