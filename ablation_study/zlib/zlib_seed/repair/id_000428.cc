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
//<ID> 428
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
    z_stream infstrm;
    memset(&infstrm, 0, sizeof(infstrm));
    int inflate_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dictionary[] = "zlib_shared_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&defstrm, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&infstrm, dictionary, dict_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence_output.gz";
    const char write_buf[] = "Payload data written to gzFile to exercise gzopen/gzputs/gzclose.";
    gzFile gz = gzopen(filename, "wb");
    int gzputs_ret = gzputs(gz, write_buf);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&defstrm);
    int inflate_end_ret = inflateEnd(&infstrm);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_init_ret;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)filename;
    (void)gzputs_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}