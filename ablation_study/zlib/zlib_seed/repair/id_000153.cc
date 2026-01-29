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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inf_strm;
    Bytef dict_buf[] = "example_dictionary_for_inflate_set_dictionary";
    uInt dict_len = (uInt)(sizeof(dict_buf) - 1);
    Bytef read_buf[512];
    z_size_t items_read;
    gzFile gz;
    const char * ver;
    int ret_inflate_init;
    int ret_setdict;
    int ret_inflate_end;
    int ret_gz_close;
    int c;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    ver = zlibVersion();
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    gz = gzopen64("test_zlib_api_sequence.gz", "rb");

    // step 3: Core operations
    items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)sizeof(read_buf), gz);
    c = gzgetc_(gz);
    inf_strm.next_in = read_buf;
    inf_strm.avail_in = (uInt)items_read;
    ret_setdict = inflateSetDictionary(&inf_strm, dict_buf, dict_len);

    // step 4: Cleanup
    ret_gz_close = gzclose(gz);
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ret_inflate_init;
    (void)ret_setdict;
    (void)ret_inflate_end;
    (void)ret_gz_close;
    (void)ver;
    (void)items_read;
    (void)c;
    (void)inf_strm.total_in;
    (void)inf_strm.total_out;
    // API sequence test completed successfully
    return 66;
}