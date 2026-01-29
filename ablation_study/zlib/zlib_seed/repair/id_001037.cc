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
//<ID> 1037
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: sample payload for compress -> inflate -> inspect.";
    uLong source_len = (uLong)(sizeof(source) - 1);

    // step 2: Configure (compress into a buffer)
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, source, source_len, 6);
    uLong comp_size = (uLong)destLen;
    (void)comp_ret;

    // step 3: Operate (inflate the compressed data and inspect codes used)
    z_stream inf_strm;
    memset(&inf_strm, 0, (size_t)sizeof(inf_strm));
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

    // step 4: Validate / Cleanup (use deflate dictionary APIs and finish inflate)
    z_stream def_strm;
    memset(&def_strm, 0, (size_t)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;
    static const Bytef preset_dict[] = "presetdict";
    uInt preset_len = (uInt)(sizeof(preset_dict) - 1);
    int setdict_ret = deflateSetDictionary(&def_strm, preset_dict, preset_len);
    (void)setdict_ret;
    Bytef *retrieved_dict = (Bytef *)malloc((size_t)preset_len);
    memset(retrieved_dict, 0, (size_t)preset_len);
    uInt retrieved_len = preset_len;
    int getdict_ret = deflateGetDictionary(&def_strm, retrieved_dict, &retrieved_len);
    (void)getdict_ret;
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_end_ret;
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)inf_end_ret;
    free(comp_buf);
    free(out_buf);
    free(retrieved_dict);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)out_len;
    (void)codes_used;
    // API sequence test completed successfully
    return 66;
}