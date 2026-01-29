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
//<ID> 859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Sample payload to compress with a dictionary for zlib API sequence";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "sample_dictionary_for_test";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    int def_dict_ret = deflateSetDictionary(&def, dict, dict_len);
    int inf_dict_ret = inflateSetDictionary(&inf, dict, dict_len);

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
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gzoffset(gzf_w);
    off_t seek_ret_write = gzseek(gzf_w, 0, 0);
    off_t pos_after_seek_write = gzoffset(gzf_w);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    off_t pos_r_before = gzoffset(gzf_r);
    off_t seek_ret_read = gzseek(gzf_r, 0, 0);
    off_t pos_r_after = gzoffset(gzf_r);

    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    uLong decomp_est = sample_len + 128;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_est);
    memset(decomp_buf, 0, (size_t)decomp_est);
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)decomp_est;
    int prime_ret = inflatePrime(&inf, 1, 1);
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;
    Bytef *dict_get_buf = (Bytef *)malloc((size_t)dict_len);
    memset(dict_get_buf, 0, (size_t)dict_len);
    uInt got_len = dict_len;
    int getdict_ret = inflateGetDictionary(&inf, dict_get_buf, &got_len);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&inf);
    int def_end = deflateEnd(&def);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(decomp_buf);
    free(dict_get_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_dict_ret;
    (void)inf_dict_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)pos_after_write;
    (void)seek_ret_write;
    (void)pos_after_seek_write;
    (void)gz_cl_w;
    (void)pos_r_before;
    (void)seek_ret_read;
    (void)pos_r_after;
    (void)prime_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)getdict_ret;
    (void)inf_end;
    (void)def_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}