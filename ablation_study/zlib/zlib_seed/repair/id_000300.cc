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
//<ID> 300
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    gzFile gz;
    const char write_buf[] = "zlib sample payload\n";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    char read_buf[128];
    char *gets_ret;
    z_stream inf_strm;
    uInt dict_len;
    int ret_inflate_init;
    int ret_setdict;
    int ret_inflate_end;
    int gz_write_ret;
    int gz_rewind_ret;
    int gz_close_ret;

    // step 2: Setup
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");

    // step 3: Core operations
    gz_write_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    gz_rewind_ret = gzrewind(gz);
    gets_ret = gzgets(gz, read_buf, (int)sizeof(read_buf));
    dict_len = (uInt)write_len;
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_setdict = inflateSetDictionary(&inf_strm, (const Bytef *)read_buf, dict_len);
    ret_inflate_end = inflateEnd(&inf_strm);
    gz_close_ret = gzclose(gz);

    // step 4: Validation / Cleanup
    (void)ver;
    (void)gets_ret;
    (void)gz_write_ret;
    (void)gz_rewind_ret;
    (void)gz_close_ret;
    (void)ret_inflate_init;
    (void)ret_setdict;
    (void)ret_inflate_end;
    // API sequence test completed successfully
    return 66;
}