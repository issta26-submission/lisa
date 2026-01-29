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
//<ID> 424
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    int deflate_init_ret = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    int inflate_init_ret = inflateInit_(&inflate_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dictionary[] = "zlib_api_sequence_dictionary";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&deflate_strm, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&inflate_strm, dictionary, dict_len);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Payload data to write via gzopen/gzwrite after dictionary set.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    int gzclose_ret = gzclose(gz);

    // step 4: Validate / Cleanup
    int deflate_end_ret = deflateEnd(&deflate_strm);
    int inflate_end_ret = inflateEnd(&inflate_strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_init_ret;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)filename;
    (void)write_len;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}