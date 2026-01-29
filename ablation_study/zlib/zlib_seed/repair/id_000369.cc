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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream inf_strm;
    Bytef dict[] = "sample-dictionary-for-inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    Bytef write_buf[] = "Zlib gzwrite payload written to a .gz file by the test function.";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz_wr;
    int ret_gzwrite;
    int ret_gzclose;
    int ret_inflate_init;
    int ret_inflate_set_dict;
    int ret_inflate_end;

    // step 2: Setup / Configure
    memset(&inf_strm, 0, sizeof(inf_strm));
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Operate - write compressed data and configure inflate dictionary
    ret_gzwrite = gzwrite(gz_wr, (voidpc)write_buf, write_len);
    ret_gzclose = gzclose(gz_wr);
    ret_inflate_init = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    ret_inflate_set_dict = inflateSetDictionary(&inf_strm, dict, dict_len);
    ret_inflate_end = inflateEnd(&inf_strm);

    // step 4: Validate / Cleanup
    (void)ver;
    (void)write_len;
    (void)ret_gzwrite;
    (void)ret_gzclose;
    (void)ret_inflate_init;
    (void)ret_inflate_set_dict;
    (void)ret_inflate_end;
    // API sequence test completed successfully
    return 66;
}