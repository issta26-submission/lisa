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
//<ID> 1030
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: use dictionary, deflate, inflate, inspect codes used.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const Bytef dict_data[] = "sample_dictionary_data_for_deflate";
    uInt dict_len = (uInt)(sizeof(dict_data) - 1);

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    (void)def_init_ret;
    int def_set_dict_ret = deflateSetDictionary(&def_strm, dict_data, dict_len);
    (void)def_set_dict_ret;

    // step 2: Configure buffers for compression
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate (deflate, retrieve dictionary, cleanup deflater)
    int def_ret = deflate(&def_strm, 0);
    uLong comp_size = def_strm.total_out;
    Bytef *retrieved_dict = (Bytef *)malloc((size_t)256);
    memset(retrieved_dict, 0, (size_t)256);
    uInt retrieved_len = (uInt)256;
    int get_dict_ret = deflateGetDictionary(&def_strm, retrieved_dict, &retrieved_len);
    (void)get_dict_ret;
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_ret;
    (void)def_end_ret;

    // step 4: Inflate compressed data, inspect codes used, cleanup
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    (void)inf_init_ret;
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_size;
    uLong out_len = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (size_t)out_len);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)out_len;
    int inf_ret = inflate(&inf_strm, 0);
    (void)inf_ret;
    unsigned long codes_used = inflateCodesUsed(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    (void)codes_used;
    (void)inf_end_ret;

    free(comp_buf);
    free(out_buf);
    free(retrieved_dict);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}