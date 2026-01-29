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
//<ID> 435
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

    // step 2: Configure (set and retrieve dictionary)
    Bytef dictionary[] = "zlib_api_sequence_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&strm, dictionary, dict_len);
    Bytef retrieved_dict[64];
    memset(retrieved_dict, 0, sizeof(retrieved_dict));
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int def_get_ret = deflateGetDictionary(&strm, retrieved_dict, &retrieved_len);

    // step 3: Operate on gz file (open, seek, clear errors, close)
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen64(filename, "wb");
    off64_t seek_offset = (off64_t)retrieved_len;
    off64_t gzseek_ret = gzseek64(gz, seek_offset, 0);
    gzclearerr(gz);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)dict_len;
    (void)def_set_ret;
    (void)retrieved_dict;
    (void)def_get_ret;
    (void)filename;
    (void)gzseek_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    // API sequence test completed successfully
    return 66;
}