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
//<ID> 431
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
    Bytef initial_dict[] = "zlib_api_sequence_dictionary_example";
    uInt initial_dict_len = (uInt)(sizeof(initial_dict) - 1);
    int def_set_ret = deflateSetDictionary(&strm, initial_dict, initial_dict_len);

    // step 2: Configure / Retrieve dictionary
    Bytef retrieved_dict[128];
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int def_get_ret = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);

    // step 3: Operate with gzFile (64-bit)
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen64(filename, "wb");
    int gzwrite_ret = gzwrite(gz, retrieved_dict, (unsigned int)retrieved_len);
    off64_t seek_pos = gzseek64(gz, (off64_t)0, 0);
    gzclearerr(gz);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)def_set_ret;
    (void)def_get_ret;
    (void)retrieved_dict;
    (void)retrieved_len;
    (void)filename;
    (void)gzwrite_ret;
    (void)seek_pos;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}