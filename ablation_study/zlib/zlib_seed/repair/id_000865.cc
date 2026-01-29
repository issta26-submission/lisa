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
//<ID> 865
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "This is a sample payload to compress using zlib for API sequencing.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "example_dictionary_for_deflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int def_set_dict_ret = deflateSetDictionary(&def, dict, dict_len);
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_setparams_ret = gzsetparams(gzf_w, 9, 1);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(readbuf, 0, (size_t)(comp_len + 16));
    z_size_t read_items = gzfread((voidp)readbuf, (z_size_t)1, (z_size_t)comp_len, gzf_r);
    uInt got_dict_len = dict_len;
    Bytef *got_dict = (Bytef *)malloc((size_t)got_dict_len);
    memset(got_dict, 0, (size_t)got_dict_len);
    int def_get_dict_ret = deflateGetDictionary(&def, got_dict, &got_dict_len);
    uLong compile_flags = zlibCompileFlags();

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(readbuf);
    free(got_dict);
    (void)version;
    (void)def_init_ret;
    (void)def_set_dict_ret;
    (void)out_bound;
    (void)gz_setparams_ret;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)read_items;
    (void)def_get_dict_ret;
    (void)got_dict_len;
    (void)compile_flags;
    (void)def_end;
    (void)gz_cl_r;
    (void)fname;
    // API sequence test completed successfully
    return 66;
}