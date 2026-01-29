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
//<ID> 304
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
    Bytef dict[] = "zlib_dictionary_payload\n";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    gzFile gzw;
    gzFile gzr;
    char readbuf[128];
    int gz_write_ret;
    char *gzgets_ret;
    int gz_close_ret1;
    int gz_close_ret2;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_end;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(readbuf, 0, sizeof(readbuf));
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 3: Core operations - set dictionary in inflate stream, write dictionary into a gz file, then read it back
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, dict, dict_len);
    gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gz_write_ret = gzwrite(gzw, (voidpc)dict, (unsigned int)dict_len);
    gz_close_ret1 = gzclose(gzw);
    gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzgets_ret = gzgets(gzr, readbuf, (int)sizeof(readbuf));
    gz_close_ret2 = gzclose(gzr);

    // step 4: Validation / cleanup
    ret_inflate_end = inflateEnd(&inf_strm);
    (void)ver;
    (void)dict_len;
    (void)gz_write_ret;
    (void)gzgets_ret;
    (void)readbuf;
    (void)gz_close_ret1;
    (void)gz_close_ret2;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_end;
    // API sequence test completed successfully
    return 66;
}