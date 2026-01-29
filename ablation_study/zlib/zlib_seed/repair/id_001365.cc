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
//<ID> 1365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "Zlib API sequence test: write then read and use as dictionary";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    gzFile gz_w = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_w, src_data, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_w = gzclose(gz_w);

    // step 2: Configure
    gzFile gz_r = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int gr = gzread(gz_r, read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_r = gzclose(gz_r);

    // step 3: Operate
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    uInt dict_cap = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int set_ret = inflateSetDictionary(&strm, read_buf, dict_cap);
    Bytef *dict_copy = (Bytef *)malloc((size_t)(dict_cap ? dict_cap : 1));
    uInt got_len = dict_cap;
    int get_ret = inflateGetDictionary(&strm, dict_copy, &got_len);
    int end_ret = inflateEnd(&strm);

    // step 4: Validate & Cleanup
    (void)version;
    (void)gw; (void)close_w; (void)gr; (void)close_r;
    (void)init_ret; (void)set_ret; (void)get_ret; (void)end_ret; (void)got_len;
    free(read_buf);
    free(dict_copy);
    // API sequence test completed successfully
    return 66;
}