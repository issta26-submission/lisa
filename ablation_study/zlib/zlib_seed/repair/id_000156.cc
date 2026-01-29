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
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    Bytef dictionary_buf[] = "example_dictionary_for_zlib_test";
    uInt dict_len = (uInt)(sizeof(dictionary_buf) - 1);
    Bytef input_buf[] = "payload that benefits from the example_dictionary_for_zlib_test dictionary usage in compression";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[4096];
    uInt comp_buf_size = (uInt)sizeof(comp_buf);
    Bytef out_buf[4096];
    uInt out_buf_size = (uInt)sizeof(out_buf);
    Bytef read_buf[4096];
    uInt read_buf_size = (uInt)sizeof(read_buf);
    const char * ver;
    int ret_def_init;
    int ret_def_setdict;
    int ret_deflate;
    int ret_def_end;
    int ret_inf_init;
    int ret_inf_setdict;
    int ret_inflate;
    int ret_inf_end;
    gzFile gz_w;
    gzFile gz_r;
    int gz_write_ret;
    z_size_t gz_read_count;
    int gz_getc_ch;
    int ret_gz_close_w;
    int ret_gz_close_r;

    // step 2: Setup (initialize streams, buffers, version)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(out_buf, 0, sizeof(out_buf));
    memset(read_buf, 0, sizeof(read_buf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    ret_def_setdict = deflateSetDictionary(&def_strm, dictionary_buf, dict_len);

    // step 3: Operate (compress with dictionary, inflate with dictionary, write/read via gzopen64)
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = comp_buf_size;
    ret_deflate = deflate(&def_strm, 0);
    ret_def_end = deflateEnd(&def_strm);

    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = out_buf_size;
    ret_inf_setdict = inflateSetDictionary(&inf_strm, dictionary_buf, dict_len);
    ret_inflate = inflate(&inf_strm, 0);
    ret_inf_end = inflateEnd(&inf_strm);

    gz_w = gzopen64("test_zlib_api_sequence_tmp.gz", "wb");
    gz_write_ret = gzwrite(gz_w, out_buf, (unsigned int)inf_strm.total_out);
    ret_gz_close_w = gzclose(gz_w);

    gz_r = gzopen64("test_zlib_api_sequence_tmp.gz", "rb");
    gz_read_count = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)read_buf_size, gz_r);
    gz_getc_ch = gzgetc_(gz_r);
    ret_gz_close_r = gzclose(gz_r);

    (void)ver;
    (void)ret_def_init;
    (void)ret_def_setdict;
    (void)ret_deflate;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inf_setdict;
    (void)ret_inflate;
    (void)ret_inf_end;
    (void)gz_write_ret;
    (void)gz_read_count;
    (void)gz_getc_ch;
    (void)ret_gz_close_w;
    (void)ret_gz_close_r;
    (void)comp_buf_size;
    (void)out_buf_size;
    (void)read_buf_size;
    (void)def_strm.total_out;
    (void)inf_strm.total_out;
    // API sequence test completed successfully
    return 66;
}