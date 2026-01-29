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
//<ID> 309
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
    char write_buf[] = "Hello zlib gzwrite test line\n";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    char read_buf[128];
    gzFile gz;
    int gz_write_ret;
    char *gz_gets_ret;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_end;

    // step 2: Setup / initialize
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    inf_strm.next_in = (Bytef *)write_buf;
    inf_strm.avail_in = (uInt)write_len;
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, (const Bytef *)write_buf, (uInt)write_len);

    // step 3: Core operations - write to a gz file, rewind, read back a line
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");
    gz_write_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    gzrewind(gz);
    gz_gets_ret = gzgets(gz, read_buf, (int)sizeof(read_buf));

    // step 4: Cleanup / validation
    ret_inflate_end = inflateEnd(&inf_strm);
    gzclose(gz);
    (void)ver;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_end;
    (void)gz_write_ret;
    (void)gz_gets_ret;
    (void)read_buf;
    // API sequence test completed successfully
    return 66;
}