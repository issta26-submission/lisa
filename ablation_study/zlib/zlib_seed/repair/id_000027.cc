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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef dictionary[] = "example_zlib_dictionary";
    Bytef dictionary_out[64];
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    uInt dict_len_out = (uInt)sizeof(dictionary_out);
    gzFile gzf;
    off64_t seek_result;
    int ret_deflate_init;
    int ret_setdict;
    int ret_getdict_before_reset;
    int ret_reset;
    int ret_getdict_after_reset;
    int ret_deflate_end;
    int ret_gzclose;
    uInt combined_metric;

    // step 2: Setup (initialize stream and open gzip file, perform 64-bit seek)
    memset(&strm, 0, sizeof(strm));
    ret_deflate_init = deflateInit_(&strm, 6, zlibVersion(), sizeof(z_stream));
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "w+b");
    seek_result = gzseek64(gzf, (off64_t)0, 0);

    // step 3: Operate & Validate (set/get dictionary, reset stream, try getting dictionary again)
    ret_setdict = deflateSetDictionary(&strm, dictionary, dict_len);
    ret_getdict_before_reset = deflateGetDictionary(&strm, dictionary_out, &dict_len_out);
    ret_reset = deflateReset(&strm);
    ret_getdict_after_reset = deflateGetDictionary(&strm, dictionary_out, &dict_len_out);
    combined_metric = dict_len_out + (uInt)(seek_result & 0xffffffffu);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_setdict;
    (void)ret_getdict_before_reset;
    (void)ret_reset;
    (void)ret_getdict_after_reset;
    (void)combined_metric;
    (void)ret_deflate_end;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}