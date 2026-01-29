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
//<ID> 1044
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Sample data for gz write and read via zlib gzputs/gzwrite/gzfread.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    Bytef dict_buf[16];
    memset(dict_buf, 'D', sizeof(dict_buf));

    // step 2: Configure deflate stream and set dictionary
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_dict_ret = deflateSetDictionary(&def_strm, dict_buf, (uInt)sizeof(dict_buf));
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate with gz I/O: write a header line and binary data
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzp_ret = gzputs(gzw, "Header-Line: zlib test\n");
    int gw_ret = gzwrite(gzw, source, (unsigned int)source_len);
    int gzf_ret = gzflush(gzw, 2);
    int gzclose_ret_w = gzclose(gzw);

    // step 4: Validate by reading back with gzfread and cleanup
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_size = (z_size_t)(source_len + 128);
    voidp read_buf = malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, read_buf_size, gzr);
    int gzclose_ret_r = gzclose(gzr);
    free(read_buf);

    (void)version;
    (void)dict_buf;
    (void)def_init_ret;
    (void)def_dict_ret;
    (void)def_end_ret;
    (void)gzp_ret;
    (void)gw_ret;
    (void)gzf_ret;
    (void)gzclose_ret_w;
    (void)items_read;
    (void)gzclose_ret_r;

    // API sequence test completed successfully
    return 66;
}