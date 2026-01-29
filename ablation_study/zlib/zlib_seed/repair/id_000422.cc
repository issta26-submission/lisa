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
//<ID> 422
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&inf_strm, 0, sizeof(inf_strm));
    int deflate_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));
    int inflate_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef dictionary[] = "zlib_shared_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);
    int def_set_ret = deflateSetDictionary(&def_strm, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&inf_strm, dictionary, dict_len);

    // step 3: Operate
    Bytef write_buf[] = "Payload data to be written to a gz file using gzopen/gzwrite.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    uLong bound_needed = deflateBound(&def_strm, (uLong)write_len);
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, write_buf, write_len);
    int gzflush_ret = gzflush(gz, 0);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&def_strm);
    int inflate_end_ret = inflateEnd(&inf_strm);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_init_ret;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)bound_needed;
    (void)filename;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}