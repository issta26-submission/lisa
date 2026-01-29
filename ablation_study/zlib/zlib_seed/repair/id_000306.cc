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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char *ver;
    z_stream inf_strm;
    unsigned char dict_arr[] = "preset_dictionary";
    uInt dict_len = (uInt)(sizeof(dict_arr) - 1);
    unsigned char payload[] = "Hello zlib gz file\n";
    unsigned int write_len = (unsigned int)(sizeof(payload) - 1);
    char read_buf[128];
    gzFile gzw;
    gzFile gzr;
    int init_ret;
    int setdict_ret;
    int gz_write_ret;
    char *gzgets_ret;
    int gzclose_ret1;
    int gzclose_ret2;
    int inf_end_ret;

    // step 2: Setup / initialization
    ver = zlibVersion();
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(read_buf, 0, sizeof(read_buf));
    init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    setdict_ret = inflateSetDictionary(&inf_strm, (const Bytef *)dict_arr, dict_len);

    // step 3: Core operations
    gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    gz_write_ret = gzwrite(gzw, (voidpc)payload, write_len);
    gzclose_ret1 = gzclose(gzw);
    gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    gzgets_ret = gzgets(gzr, read_buf, (int)sizeof(read_buf));
    gzclose_ret2 = gzclose(gzr);

    // step 4: Validation / cleanup
    inf_end_ret = inflateEnd(&inf_strm);
    (void)ver;
    (void)init_ret;
    (void)setdict_ret;
    (void)dict_len;
    (void)write_len;
    (void)gz_write_ret;
    (void)gzgets_ret;
    (void)gzclose_ret1;
    (void)gzclose_ret2;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}