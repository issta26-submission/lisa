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
//<ID> 401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    Bytef dict[] = "zlib_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    int deflate_set_dict_ret = deflateSetDictionary(&strm, dict, dict_len);

    // step 2: Setup / Configure
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Payload for gzwrite followed by gzoffset64 and gzeof checks.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 65536U);

    // step 3: Operate
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    off64_t offset = gzoffset64(gz);
    int eof_flag = gzeof(gz);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)deflate_set_dict_ret;
    (void)filename;
    (void)write_len;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)offset;
    (void)eof_flag;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}