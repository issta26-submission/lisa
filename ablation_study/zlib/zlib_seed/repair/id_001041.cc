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
//<ID> 1041
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: compress data, write with gzwrite/gzputs, read with gzfread, and init a deflate stream.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);
    (void)comp_ret;

    // step 2: Configure
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_len);
    int gzputs_ret = gzputs(gzw, "Extra-line-for-gzputs\n");
    int gzflush_ret = gzflush(gzw, 2);
    int gzclose_w_ret = gzclose(gzw);
    (void)gzwrite_ret; (void)gzputs_ret; (void)gzflush_ret; (void)gzclose_w_ret;

    // step 3: Operate (read back)
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_size = (z_size_t)(source_len + comp_len + 64);
    void *read_buf = malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    z_size_t items_read = gzfread(read_buf, 1, read_buf_size, gzr);
    (void)items_read;

    // step 4: Validate / Cleanup
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_end_ret = deflateEnd(&def_strm);
    int gzclose_r_ret = gzclose(gzr);
    free(comp_buf);
    free(read_buf);
    (void)def_init_ret; (void)def_end_ret; (void)gzclose_r_ret; (void)version; (void)source_len; (void)bound; (void)comp_len;
    // API sequence test completed successfully
    return 66;
}