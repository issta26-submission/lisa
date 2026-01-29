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
//<ID> 854
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "This is a test sample for zlib usage.";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "preset_dictionary_example";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure / Compress / Write
    int set_dict_ret = deflateSetDictionary(&def, dict, dict_len);
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    const char *fname = "zlib_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzoffset(gzf_w);
    off_t seek_ret = gzseek(gzf_w, 0, 0);
    int gz_cl_w = gzclose(gzf_w);

    // step 3: Operate / Inflate with priming and retrieve dictionary
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 64));
    memset(decomp_buf, 0, (size_t)(sample_len + 64));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 64);
    int prime_ret = inflatePrime(&inf, 0, 0);
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;
    Bytef dict_out[64];
    memset(dict_out, 0, sizeof(dict_out));
    uInt dict_out_len = (uInt)sizeof(dict_out);
    int getdict_ret = inflateGetDictionary(&inf, dict_out, &dict_out_len);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&inf);
    int def_end = deflateEnd(&def);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init;
    (void)out_bound;
    (void)set_dict_ret;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)pos_after_write;
    (void)seek_ret;
    (void)gz_cl_w;
    (void)inf_init;
    (void)prime_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)getdict_ret;
    (void)dict_out_len;
    (void)inf_end;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}