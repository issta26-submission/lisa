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
//<ID> 400
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
    Bytef dictionary[] = "example_dictionary_for_deflate";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    Bytef write_buf[] = "Example payload data to write into gzip file using gzwrite.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    int init_ret;
    int setdict_ret;
    int end_ret;
    gzFile gz_wr;
    int gzbuffer_ret;
    int gzwrite_ret;
    off64_t offset_ret;
    int eof_ret;
    int gzclose_ret;

    // step 2: Setup / Configure
    init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    setdict_ret = deflateSetDictionary(&strm, dictionary, dict_len);

    // step 3: Operate
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzbuffer_ret = gzbuffer(gz_wr, 8192u);
    gzwrite_ret = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    offset_ret = gzoffset64(gz_wr);
    eof_ret = gzeof(gz_wr);

    // step 4: Validate / Cleanup
    end_ret = deflateEnd(&strm);
    gzclose_ret = gzclose(gz_wr);
    (void)ver;
    (void)init_ret;
    (void)setdict_ret;
    (void)end_ret;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)offset_ret;
    (void)eof_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}