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
//<ID> 406
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
    Bytef write_buf[] = "Example payload that also serves as a dictionary prefix for deflateSetDictionary and as gzwrite data.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int init_ret;
    int setdict_ret;
    int buf_ret;
    int write_ret;
    off64_t off_ret;
    int eof_ret;
    int end_ret;
    int close_ret;

    // step 2: Setup / Configure deflate and dictionary
    init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    /* Use the beginning of the write buffer as the dictionary to demonstrate meaningful data flow */
    setdict_ret = deflateSetDictionary(&strm, write_buf, (uInt)16);

    // step 3: Operate / configure gz buffering and write compressed data
    buf_ret = gzbuffer(gz, 8192);
    write_ret = gzwrite(gz, (voidpc)write_buf, write_len);

    // step 4: Validate / Cleanup
    off_ret = gzoffset64(gz);
    eof_ret = gzeof(gz);
    end_ret = deflateEnd(&strm);
    close_ret = gzclose(gz);
    (void)ver;
    (void)init_ret;
    (void)setdict_ret;
    (void)buf_ret;
    (void)write_ret;
    (void)off_ret;
    (void)eof_ret;
    (void)end_ret;
    (void)close_ret;
    // API sequence test completed successfully
    return 66;
}