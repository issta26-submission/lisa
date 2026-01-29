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
//<ID> 989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: produce compressed data for inflateBack test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;

    // step 2: Configure
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_ret;
    (void)def_end_ret;

    // step 4: Validate / Cleanup
    z_stream inf_back_strm;
    memset(&inf_back_strm, 0, sizeof(inf_back_strm));
    unsigned char *window = (unsigned char *)malloc((size_t)(1 << 15));
    memset(window, 0, (size_t)(1 << 15));
    int back_init_ret = inflateBackInit_(&inf_back_strm, 15, window, version, (int)sizeof(z_stream));
    (void)back_init_ret;

    inf_back_strm.next_in = comp_buf;
    inf_back_strm.avail_in = (uInt)comp_size;

    int prime_ret = inflatePrime(&inf_back_strm, 1, 1);
    (void)prime_ret;

    uInt dictLength = (uInt)source_len;
    Bytef *dictBuf = (Bytef *)malloc((size_t)dictLength);
    memset(dictBuf, 0, (size_t)dictLength);
    int getdict_ret = inflateGetDictionary(&inf_back_strm, dictBuf, &dictLength);
    (void)getdict_ret;
    (void)dictLength;

    int back_end_ret = inflateBackEnd(&inf_back_strm);
    (void)back_end_ret;

    free(comp_buf);
    free(window);
    free(dictBuf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}