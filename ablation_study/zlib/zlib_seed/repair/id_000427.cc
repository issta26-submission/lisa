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
//<ID> 427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream defl;
    z_stream infl;
    memset(&defl, 0, sizeof(defl));
    memset(&infl, 0, sizeof(infl));
    int deflate_init_ret = deflateInit_(&defl, 6, ver, (int)sizeof(z_stream));
    int inflate_init_ret = inflateInit_(&infl, ver, (int)sizeof(z_stream));
    Bytef dictionary[] = "zlib_sequence_dictionary";
    uInt dict_len = (uInt)(sizeof(dictionary) - 1);

    // step 2: Configure
    int def_set_ret = deflateSetDictionary(&defl, dictionary, dict_len);
    int inf_set_ret = inflateSetDictionary(&infl, dictionary, dict_len);
    uLong bound_estimate = deflateBound(&defl, (uLong)256);

    // step 3: Operate
    const char *filename = "test_zlib_api_sequence.tmp.gz";
    Bytef write_buf[] = "Zlib API sequence payload for gzwrite and dictionary test.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz = gzopen(filename, "wb");
    int gzbuffer_ret = gzbuffer(gz, 8192U);
    int gzwrite_ret = gzwrite(gz, (voidpc)write_buf, write_len);
    int gzflush_ret = gzflush(gz, 0);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&defl);
    int inflate_end_ret = inflateEnd(&infl);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_init_ret;
    (void)def_set_ret;
    (void)inf_set_ret;
    (void)bound_estimate;
    (void)filename;
    (void)write_len;
    (void)gzbuffer_ret;
    (void)gzwrite_ret;
    (void)gzflush_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_end_ret;
    // API sequence test completed successfully
    return 66;
}