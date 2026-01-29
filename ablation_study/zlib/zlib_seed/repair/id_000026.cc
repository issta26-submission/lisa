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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef dictionary[] = "example dictionary for zlib";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    Bytef retrieved_dict[64];
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    gzFile gzf;
    off64_t seek_result;
    int ret_init;
    int ret_setdict;
    int ret_getdict;
    int ret_reset;
    int ret_deflate_end;
    int ret_gzclose;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    ret_init = deflateInit_(&strm, 6, zlibVersion(), sizeof(z_stream));
    ret_setdict = deflateSetDictionary(&strm, dictionary, dict_len);

    // step 3: Operate & Validate
    ret_getdict = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);
    ret_reset = deflateReset(&strm);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    seek_result = gzseek64(gzf, (off64_t)retrieved_len, 0);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    // API sequence test completed successfully
    (void)ret_init; (void)ret_setdict; (void)ret_getdict; (void)ret_reset; (void)seek_result; (void)ret_deflate_end; (void)ret_gzclose;
    return 66;
}