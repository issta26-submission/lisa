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
//<ID> 868
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "This is a sample payload to compress using zlib with a preset dictionary.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "preset_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int setdict_ret = deflateSetDictionary(&def, dict, dict_len);
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_set = gzsetparams(gzf_w, 9, 0);
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(read_buf, 0, (size_t)(comp_len + 16));
    z_size_t items_read = gzfread((voidp)read_buf, (z_size_t)1, (z_size_t)comp_len, gzf_r);
    uLong compile_flags = zlibCompileFlags();
    Bytef dict_get_buf[128];
    memset(dict_get_buf, 0, (size_t)sizeof(dict_get_buf));
    uInt got_len = (uInt)sizeof(dict_get_buf);
    int getdict_ret = deflateGetDictionary(&def, dict_get_buf, &got_len);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)setdict_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)gzf_w;
    (void)gz_set;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)items_read;
    (void)compile_flags;
    (void)getdict_ret;
    (void)got_len;
    (void)dict_get_buf;
    (void)def_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}