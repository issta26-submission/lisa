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
//<ID> 151
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
    Bytef input_buf[] = "zlib API sequence payload with preset dictionary to exercise deflate/inflate and gz I/O.";
    Bytef dict_buf[] = "preset_dictionary_data";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef comp_buf[4096];
    uInt comp_buf_size = (uInt)sizeof(comp_buf);
    Bytef read_buf[4096];
    uInt read_buf_size = (uInt)sizeof(read_buf);
    Bytef out_buf[4096];
    uInt out_buf_size = (uInt)sizeof(out_buf);
    const char * ver;
    int ret_def_init;
    int ret_def_setdict;
    int ret_deflate;
    int ret_def_end;
    int ret_inf_init;
    int ret_inf_setdict;
    int ret_inflate;
    int ret_inf_end;
    gzFile gz;
    int gz_write_ret;
    z_size_t gzread_ret;
    int gzgetc_ret;
    int ret_gz_close;

    // step 2: Setup (initialize streams, set dictionary, compress)
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(read_buf, 0, sizeof(read_buf));
    memset(out_buf, 0, sizeof(out_buf));
    ver = zlibVersion();
    ret_def_init = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)input_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = comp_buf_size;
    ret_def_setdict = deflateSetDictionary(&def_strm, dict_buf, dict_len);
    ret_deflate = deflate(&def_strm, 0);
    uLong comp_used = def_strm.total_out;

    // step 3: Write compressed data to a gz file and read it back
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gz_write_ret = gzwrite(gz, comp_buf, (unsigned int)comp_used);
    ret_gz_close = gzclose(gz);
    gz = gzopen64("test_zlib_api_sequence.gz", "rb");
    gzread_ret = gzfread(read_buf, 1, (z_size_t)comp_used, gz);
    gzgetc_ret = gzgetc_(gz);

    // step 4: Inflate with same dictionary and cleanup
    ret_inf_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)gzread_ret;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = out_buf_size;
    ret_inf_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);
    ret_inflate = inflate(&inf_strm, 0);
    ret_inf_end = inflateEnd(&inf_strm);
    ret_def_end = deflateEnd(&def_strm);
    ret_gz_close = gzclose(gz);
    (void)ret_def_init;
    (void)ret_def_setdict;
    (void)ret_deflate;
    (void)ret_def_end;
    (void)ret_inf_init;
    (void)ret_inf_setdict;
    (void)ret_inflate;
    (void)ret_inf_end;
    (void)gz_write_ret;
    (void)gzread_ret;
    (void)gzgetc_ret;
    (void)ret_gz_close;
    (void)ver;
    (void)comp_buf_size;
    (void)read_buf_size;
    (void)out_buf_size;
    (void)comp_used;
    // API sequence test completed successfully
    return 66;
}