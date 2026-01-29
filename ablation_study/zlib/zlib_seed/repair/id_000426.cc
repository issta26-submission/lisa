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
//<ID> 426
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
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    z_stream instrm;
    memset(&instrm, 0, sizeof(instrm));
    int inf_init_ret = inflateInit_(&instrm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dictionary[] = "zlib_test_dictionary_sequence";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&defstrm, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&instrm, dictionary, dict_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gzwrite and deflateBound test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    uLong bound_needed = deflateBound(&defstrm, (uLong)write_len);
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    int gzflush_ret = gzflush(gz, 0);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_end_ret = inflateEnd(&instrm);
    (void)ver;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)filename;
    (void)write_len;
    (void)bound_needed;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}