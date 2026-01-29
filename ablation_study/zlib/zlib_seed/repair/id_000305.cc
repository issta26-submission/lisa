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
//<ID> 305
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
    z_stream inf_strm;
    Bytef write_buf[] = "preset_dictionary_line\n";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    char read_buf[256];
    int gzwrite_ret;
    char *gzgets_ret;
    int gzclose_ret;
    int ret_inflate_init;
    int ret_inflate_setdict;
    int ret_inflate_end;
    int gzflush_ret;
    off_t seek_ret;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    gz = gzopen("test_zlib_api_sequence.gz", "wb+");

    // step 3: Core operations - initialize inflate, write to gz, rewind and read, set dictionary, cleanup inflate
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    gzflush_ret = gzflush(gz, 0);
    seek_ret = gzseek(gz, (off_t)0, 0);
    gzgets_ret = gzgets(gz, read_buf, (int)sizeof(read_buf));
    ret_inflate_setdict = inflateSetDictionary(&inf_strm, (const Bytef *)read_buf, (uInt)write_len);
    ret_inflate_end = inflateEnd(&inf_strm);
    gzclose_ret = gzclose(gz);

    // step 4: Validation / cleanup
    (void)ver;
    (void)gzwrite_ret;
    (void)gzgets_ret;
    (void)gzclose_ret;
    (void)ret_inflate_init;
    (void)ret_inflate_setdict;
    (void)ret_inflate_end;
    (void)gzflush_ret;
    (void)seek_ret;
    (void)read_buf;
    // API sequence test completed successfully
    return 66;
}