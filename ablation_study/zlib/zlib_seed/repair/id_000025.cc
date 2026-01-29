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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef dict_in[] = "zlib_dictionary_sample_data";
    uInt dict_in_len = (uInt)(sizeof(dict_in) - 1);
    Bytef dict_out[128];
    uInt dict_out_len = (uInt)sizeof(dict_out);
    gzFile gzf;
    off64_t seek_res;
    int ret_init;
    int ret_set;
    int ret_get;
    int ret_reset;
    int ret_end;
    int ret_gzclose;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    ret_init = deflateInit_(&strm, 6, zlibVersion(), sizeof(z_stream));
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "w+b");

    // step 3: Core operations (configure dictionary, retrieve it, reset stream, seek file)
    ret_set = deflateSetDictionary(&strm, dict_in, dict_in_len);
    ret_get = deflateGetDictionary(&strm, dict_out, &dict_out_len);
    ret_reset = deflateReset(&strm);
    seek_res = gzseek64(gzf, (off64_t)0, 0);

    // step 4: Cleanup
    ret_end = deflateEnd(&strm);
    ret_gzclose = gzclose_r(gzf);
    (void)ret_init; (void)ret_set; (void)ret_get; (void)ret_reset; (void)seek_res; (void)ret_end; (void)ret_gzclose; (void)dict_out_len; (void)dict_out;

    // API sequence test completed successfully
    return 66;
}