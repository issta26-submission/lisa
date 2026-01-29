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
//<ID> 1033
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure deflate
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: deflate -> deflateGetDictionary -> inflate -> inspect codes used.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Operate deflate and query dictionary
    int def_ret = deflate(&def_strm, 0);
    (void)def_ret;
    uLong comp_size = def_strm.total_out;
    Bytef *dict_buf = (Bytef *)malloc((size_t)bound);
    memset(dict_buf, 0, (size_t)bound);
    uInt dict_len = (uInt)bound;
    int def_get_dict_ret = deflateGetDictionary(&def_strm, dict_buf, &dict_len);
    (void)def_get_dict_ret;

    // step 3: Initialize / Operate inflate and inspect codes used
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    (void)inf_init_ret;
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

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)inf_end_ret;
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_end_ret;
    free(comp_buf);
    free(out_buf);
    free(dict_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}