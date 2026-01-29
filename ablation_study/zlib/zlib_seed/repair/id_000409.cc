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
//<ID> 409
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
    int deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "zlib_test_dictionary";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);

    // step 2: Configure
    int dict_set_ret = deflateSetDictionary(&strm, dictionary, dict_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gzwrite, gzbuffer, gzoffset64 and gzeof test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    off64_t offset_after_write = gzoffset64(gz);
    int eof_flag = gzeof(gz);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)dict_set_ret;
    (void)filename;
    (void)write_len;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)offset_after_write;
    (void)eof_flag;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}