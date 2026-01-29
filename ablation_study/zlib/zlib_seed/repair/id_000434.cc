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
//<ID> 434
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

    // step 2: Configure
    Bytef dictionary[] = "example_dictionary_for_deflate_get";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&strm, dictionary, dict_len);
    Bytef retrieved_dict[128];
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int def_get_ret = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen64(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    off64_t seek_end_ret = gzseek64(gz, (off64_t)0, 2);
    gzclearerr(gz);
    int gzwrite_ret = gzwrite(gz, (voidpc)retrieved_dict, (unsigned int)retrieved_len);
    off64_t seek_start_ret = gzseek64(gz, (off64_t)0, 0);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)def_set_ret;
    (void)def_get_ret;
    (void)retrieved_len;
    (void)filename;
    (void)gzbuffer_ret;
    (void)seek_end_ret;
    (void)gzwrite_ret;
    (void)seek_start_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}