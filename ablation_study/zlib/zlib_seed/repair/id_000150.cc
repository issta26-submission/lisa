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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef preset_dict[] = "preset_dictionary_for_inflate";
    uInt preset_dict_len = (uInt)(sizeof(preset_dict) - 1);
    Bytef in_buf[1024];
    uInt in_buf_size = (uInt)sizeof(in_buf);
    Bytef out_buf[1024];
    uInt out_buf_size = (uInt)sizeof(out_buf);
    const char * ver;
    gzFile gz;
    z_size_t read_count;
    int gz_write_ret;
    int gz_close_ret1;
    int gz_close_ret2;
    int c_ch;
    int ret_inflate_init;
    int ret_set_dict;
    int ret_inflate;
    int ret_inflate_end;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(in_buf, 0, sizeof(in_buf));
    memset(out_buf, 0, sizeof(out_buf));
    ver = zlibVersion();
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 3: Core operations
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gz_write_ret = gzwrite(gz, preset_dict, (unsigned int)preset_dict_len);
    gz_close_ret1 = gzclose(gz);
    gz = gzopen64("test_zlib_api_sequence.gz", "rb");
    read_count = gzfread(in_buf, 1, in_buf_size, gz);
    c_ch = gzgetc_(gz);
    ret_set_dict = inflateSetDictionary(&inf_strm, preset_dict, preset_dict_len);
    inf_strm.next_in = in_buf;
    inf_strm.avail_in = (uInt)read_count;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = out_buf_size;
    ret_inflate = inflate(&inf_strm, 0);

    // step 4: Cleanup
    ret_inflate_end = inflateEnd(&inf_strm);
    gz_close_ret2 = gzclose(gz);
    (void)ret_inflate_init;
    (void)ret_set_dict;
    (void)ret_inflate;
    (void)ret_inflate_end;
    (void)ver;
    (void)gz_write_ret;
    (void)gz_close_ret1;
    (void)gz_close_ret2;
    (void)read_count;
    (void)c_ch;
    (void)in_buf_size;
    (void)out_buf_size;
    (void)inf_strm.total_in;
    (void)inf_strm.total_out;
    // API sequence test completed successfully
    return 66;
}