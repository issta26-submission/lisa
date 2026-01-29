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
//<ID> 407
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    Bytef dict[] = "example_dictionary_for_deflateSetDictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    Bytef write_buf[] = "Payload written to gzFile after configuring deflate dictionary.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    int init_ret;
    int dict_ret;
    int gzbuffer_ret;
    int gzwrite_ret;
    off64_t offset_ret;
    int eof_ret;
    int deflate_end_ret;
    const char *filename = "test_zlib_api_sequence.tmp.gz";

    // step 2: Configure
    init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    dict_ret = deflateSetDictionary(&strm, dict, dict_len);

    // step 3: Operate
    gz_wr = gzopen(filename, "wb");
    gzbuffer_ret = gzbuffer(gz_wr, 8192U);
    gzwrite_ret = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    offset_ret = gzoffset64(gz_wr);
    eof_ret = gzeof(gz_wr);

    // step 4: Validate / Cleanup
    gzclose(gz_wr);
    deflate_end_ret = deflateEnd(&strm);
    (void)version;
    (void)init_ret;
    (void)dict_ret;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)offset_ret;
    (void)eof_ret;
    (void)deflate_end_ret;
    (void)filename;
    // API sequence test completed successfully
    return 66;
}