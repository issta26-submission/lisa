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
//<ID> 301
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    gzFile gzw;
    gzFile gzr;
    Bytef dict[] = "example-dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    Bytef write_buf[] = "Hello zlib gzwrite/gzgets test\n";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    char read_buf[128];
    z_stream inf_strm;
    int gzwrite_ret;
    char *gzgets_ret;
    int gzclose_ret1;
    int gzclose_ret2;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_end;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    gzw = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 3: Core operations - write, close, inflate set dictionary, reopen, read
    gzwrite_ret = gzwrite(gzw, (voidpc)write_buf, write_len);
    gzclose_ret1 = gzclose(gzw);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict, dict_len);
    ret_inflate_end = inflateEnd(&inf_strm);
    gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzgets_ret = gzgets(gzr, read_buf, (int)sizeof(read_buf));
    gzclose_ret2 = gzclose(gzr);

    // step 4: Validation / cleanup
    (void)ver;
    (void)dict_len;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzclose_ret1;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_end;
    (void)gzgets_ret;
    (void)read_buf;
    (void)gzclose_ret2;
    // API sequence test completed successfully
    return 66;
}