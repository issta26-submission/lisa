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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    Bytef data[] = "Example payload for zlib API sequence: using deflate dictionary and gz write/offset/eof/buffer.";
    uInt data_len = (uInt)(sizeof(data) - 1);
    uInt dict_len = (uInt)8;
    Bytef *dictionary = data;
    gzFile gz;
    int init_ret;
    int setdict_ret;
    int end_ret;
    int gzbuffer_ret;
    int gzwrite_ret;
    off64_t offset_ret;
    int eof_ret;
    int gzclose_ret;

    // step 2: Setup / Configure
    init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    setdict_ret = deflateSetDictionary(&dstrm, dictionary, dict_len);
    gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzbuffer_ret = gzbuffer(gz, 4096);

    // step 3: Operate
    gzwrite_ret = gzwrite(gz, (voidpc)data, (unsigned int)data_len);
    offset_ret = gzoffset64(gz);
    eof_ret = gzeof(gz);

    // step 4: Validate / Cleanup
    gzclose_ret = gzclose(gz);
    end_ret = deflateEnd(&dstrm);
    (void)ver;
    (void)init_ret;
    (void)setdict_ret;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)offset_ret;
    (void)eof_ret;
    (void)gzclose_ret;
    (void)end_ret;
    (void)data_len;
    (void)dict_len;
    (void)dictionary;
    // API sequence test completed successfully
    return 66;
}