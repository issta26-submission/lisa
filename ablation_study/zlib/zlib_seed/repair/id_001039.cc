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
//<ID> 1039
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence test data: compress then inflate, inspect codes used and dictionaries.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, source, source_len, 6);
    uLong comp_size = (uLong)destLen;
    (void)comp_ret;

    // step 2: Configure deflate stream and query dictionary
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    Bytef dict_buf[64];
    memset(dict_buf, 0, sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int getdict_ret = deflateGetDictionary(&def_strm, dict_buf, &dict_len);
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_init_ret;
    (void)getdict_ret;
    (void)def_end_ret;

    // step 3: Configure inflater and operate
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    uLong out_len = source_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (size_t)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_len;
    int inf_ret = inflate(&inf_strm, 0);
    (void)inf_ret;
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    (void)codes_used;
    (void)inf_init_ret;

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)out_len;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}