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
//<ID> 437
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
    Bytef retrieved_dict[64];
    memset(retrieved_dict, 0, sizeof(retrieved_dict));
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int def_get_ret = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen64(filename, "wb");
    int gzputs_ret = gzputs(gz, (const char *)dictionary);
    off64_t seek_pos = gzseek64(gz, (off64_t)0, 0);
    gzclearerr(gz);

    // step 4: Validate / Cleanup
    uLong dict_adler = adler32(0UL, retrieved_dict, retrieved_len);
    int deflate_end_ret = deflateEnd(&strm);
    int gzclose_ret = gzclose(gz);
    (void)ver;
    (void)deflate_init_ret;
    (void)def_set_ret;
    (void)def_get_ret;
    (void)gzputs_ret;
    (void)seek_pos;
    (void)dict_adler;
    (void)deflate_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}