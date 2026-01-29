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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream defstrm;
    z_stream instrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&instrm, ver, (int)sizeof(z_stream));

    // Step 2: Configure
    Bytef dictionary[] = "zlib_shared_dictionary_payload";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&defstrm, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&instrm, dictionary, dict_len);

    // Step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    Bytef write_buf[] = "Sample compressed payload using shared dictionary.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    int gzflush_ret = gzflush(gz, 1);
    int gzclose_ret = gzclose(gz);

    // Step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&instrm);
    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)filename;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)write_len;
    // API sequence test completed successfully
    return 66;
}