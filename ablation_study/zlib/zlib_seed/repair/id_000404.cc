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
//<ID> 404
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
    Bytef dict[] = "example_dict_for_deflateSetDictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    Bytef write_buf[] = "Payload for gzwrite using zlib API sequence test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    int deflate_init_ret;
    int setdict_ret;
    int gzbuffer_ret;
    int gzwrite_ret;
    off64_t offset_ret;
    int eof_ret;
    int deflate_end_ret;
    int gzclose_ret;

    // step 2: Setup / Configure zlib deflate state and dictionary
    deflate_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    setdict_ret = deflateSetDictionary(&strm, dict, dict_len);

    // step 3: Operate / Open gz file, set buffer, write data and query state
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzbuffer_ret = gzbuffer(gz_wr, 8192);
    gzwrite_ret = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    offset_ret = gzoffset64(gz_wr);
    eof_ret = gzeof(gz_wr);
    gzclose_ret = gzclose(gz_wr);

    // step 4: Validate / Cleanup
    deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)setdict_ret;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)offset_ret;
    (void)eof_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}