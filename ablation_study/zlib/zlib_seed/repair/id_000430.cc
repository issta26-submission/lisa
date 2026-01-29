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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int deflate_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "zlib_fuzz_dictionary_for_test_unique";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&defstrm, dictionary, dict_len);

    // step 2: Configure
    Bytef retrieved_dict[128];
    uInt retrieved_len = (uInt)sizeof(retrieved_dict);
    int def_get_ret = deflateGetDictionary(&defstrm, retrieved_dict, &retrieved_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen64(filename, "wb");
    unsigned int write_len = (unsigned int)retrieved_len;
    int gzwrite_ret = gzwrite(gz, (voidpc)retrieved_dict, write_len);
    off64_t seek_ret = gzseek64(gz, (off64_t)0, 0);
    gzclearerr(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&defstrm);
    int gzclose_ret = gzclose(gz);
    (void)ver;
    (void)deflate_init_ret;
    (void)dict_len;
    (void)def_set_ret;
    (void)def_get_ret;
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)seek_ret;
    (void)deflate_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}