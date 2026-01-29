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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef comp_buf[4096];
    uInt comp_buf_size = (uInt)sizeof(comp_buf);
    Bytef out_buf[4096];
    uInt out_buf_size = (uInt)sizeof(out_buf);
    Bytef dict[] = "preset_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    const char * ver = zlibVersion();
    gzFile gz;
    z_size_t read_items;
    int single_char;
    int ret_inflate_init;
    int ret_setdict;
    int ret_inflate;
    int ret_inflate_end;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(out_buf, 0, sizeof(out_buf));
    gz = gzopen64("test_zlib_api_sequence.gz", "rb");

    // step 3: Core operations
    single_char = gzgetc_(gz);
    read_items = gzfread(comp_buf, 1, comp_buf_size, gz);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)read_items;
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = out_buf_size;
    ret_setdict = inflateSetDictionary(&inf_strm, dict, dict_len);
    ret_inflate = inflate(&inf_strm, 0);

    // step 4: Cleanup
    ret_inflate_end = inflateEnd(&inf_strm);
    gzclose(gz);
    (void)single_char;
    (void)read_items;
    (void)ret_inflate_init;
    (void)ret_setdict;
    (void)ret_inflate;
    (void)ret_inflate_end;
    (void)ver;
    (void)comp_buf_size;
    (void)out_buf_size;
    (void)inf_strm.total_out;
    // API sequence test completed successfully
    return 66;
}