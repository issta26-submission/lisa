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
//<ID> 1048
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence sample data for compress, gz write/read and deflate init usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, source, source_len, 6);
    uLong comp_size = (uLong)destLen;
    (void)comp_ret;
    (void)version;

    // step 2: Configure
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = comp_buf;
    def_strm.avail_in = (uInt)comp_size;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_init_ret;
    (void)def_end_ret;

    // step 3: Operate
    gzFile gzw = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gzwrite_ret1 = gzwrite(gzw, source, (unsigned int)source_len);
    int gzwrite_ret2 = gzwrite(gzw, comp_buf, (unsigned int)(comp_size < (uLong)1024 ? comp_size : (uLong)1024));
    int gzputs_ret = gzputs(gzw, "===footer===\n");
    int gzclose_ret_w = gzclose(gzw);
    (void)gzwrite_ret1;
    (void)gzwrite_ret2;
    (void)gzputs_ret;
    (void)gzclose_ret_w;

    // step 4: Validate & Cleanup
    gzFile gzr = gzopen("tmp_zlib_api_sequence.gz", "rb");
    z_size_t total_read_space = (z_size_t)(source_len + comp_size + 64);
    voidp read_buf = malloc((size_t)total_read_space);
    memset(read_buf, 0, (size_t)total_read_space);
    z_size_t items_read = gzfread(read_buf, 1, total_read_space, gzr);
    int gzclose_ret_r = gzclose(gzr);
    free(comp_buf);
    free(read_buf);
    (void)items_read;
    (void)gzclose_ret_r;
    // API sequence test completed successfully
    return 66;
}