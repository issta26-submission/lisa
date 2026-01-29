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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream inf_strm;
    Bytef dict[] = "preset_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    Bytef write_buf[] = "Hello zlib gzwrite and gzgets test\n";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    char read_buf[128];
    gzFile gz;
    int ret_inflate_init;
    int ret_set_dict;
    char *read_ptr;
    int ret_rewind;
    int ret_close;
    int ret_inflate_end;
    int gz_write_ret;

    // step 2: Setup
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    ver = zlibVersion();
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");

    // step 3: Core operations
    gz_write_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    ret_rewind = gzrewind(gz);
    read_ptr = gzgets(gz, read_buf, (int)sizeof(read_buf));
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_set_dict = inflateSetDictionary(&inf_strm, dict, dictLen);
    ret_inflate_end = inflateEnd(&inf_strm);
    ret_close = gzclose(gz);

    // step 4: Validation / cleanup
    (void)ver;
    (void)gz_write_ret;
    (void)read_ptr;
    (void)read_buf[0];
    (void)ret_rewind;
    (void)ret_inflate_init;
    (void)ret_set_dict;
    (void)ret_inflate_end;
    (void)ret_close;
    // API sequence test completed successfully
    return 66;
}