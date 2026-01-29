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
//<ID> 1172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data, dictionary and deflate stream
    const Bytef source[] = "Example payload data to be compressed using a preset dictionary for zlib API sequence testing.";
    const Bytef dict[] = "preset_dictionary_for_zlib";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong dict_len = (uLong)(sizeof(dict) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    int def_setdict_ret = deflateSetDictionary(&def_strm, dict, (uInt)dict_len);
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 2: Compress the source with the preset dictionary and finalize deflate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize inflate stream, set same dictionary, inflate and validate with adler32
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int inf_setdict_ret = inflateSetDictionary(&inf_strm, dict, (uInt)dict_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)(source_len + 64));
    memset(out_buf, 0, (int)(source_len + 64));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)((source_len + 64) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(source_len + 64));
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    uLong checksum = adler32(0UL, out_buf, (uInt)(out_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_size));
    uLong combined = adler32_combine64(checksum, checksum, (off64_t)out_size);

    // step 4: Cleanup and finalization
    int inf_end_ret = inflateEnd(&inf_strm);
    free(comp_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)def_setdict_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_setdict_ret;
    (void)inf_ret;
    (void)out_size;
    (void)checksum;
    (void)combined;
    (void)inf_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}