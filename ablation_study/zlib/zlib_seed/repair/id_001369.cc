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
//<ID> 1369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test data: write with gzwrite, read with gzread, use as inflate dictionary";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw1 = gzwrite(gzw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_w1 = gzclose(gzw);
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(read_buf, 0, (int)(src_len + 1));
    int gr = gzread(gzr, read_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int close_r = gzclose(gzr);

    // step 2: Configure
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = inflateInit_(&strm, version, (int)sizeof(z_stream));
    uInt dict_len = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int set_ret = inflateSetDictionary(&strm, read_buf, dict_len);

    // step 3: Operate
    Bytef *dict_out = (Bytef *)malloc((size_t)dict_len);
    memset(dict_out, 0, (int)dict_len);
    uInt out_len = dict_len;
    int get_ret = inflateGetDictionary(&strm, dict_out, &out_len);
    gzFile gzdict = gzopen("test_zlib_api_sequence_dict.gz", "wb");
    int gw2 = gzwrite(gzdict, dict_out, (unsigned int)(out_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)out_len));
    int close_w2 = gzclose(gzdict);

    // step 4: Validate & Cleanup
    (void)gw1;
    (void)close_w1;
    (void)gr;
    (void)close_r;
    (void)init_ret;
    (void)set_ret;
    (void)get_ret;
    (void)gw2;
    (void)close_w2;
    (void)version;
    inflateEnd(&strm);
    free(read_buf);
    free(dict_out);
    // API sequence test completed successfully
    return 66;
}