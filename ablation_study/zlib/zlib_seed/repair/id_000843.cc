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
//<ID> 843
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example data to compress and decompress with zlib";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "preset_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_def = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)(out_bound + 16));
    memset(comp_buf, 0, (size_t)(out_bound + 16));
    def.next_out = comp_buf;
    def.avail_out = (uInt)(out_bound + 16);

    // step 2: Configure
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int init_inf = inflateInit_(&inf, version, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 1024));
    memset(decomp_buf, 0, (size_t)(sample_len + 1024));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 1024);
    int dict_set_ret = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    unsigned int pending = 0;
    int pending_bits = 0;
    int pend_ret = deflatePending(&def, &pending, &pending_bits);
    uLong comp_len = def.total_out;
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = 0;
    if (gzf_w) gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = 0;
    if (gzf_w) gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(read_buf, 0, (size_t)(comp_len + 16));
    int gz_rd = 0;
    if (gzf_r) gz_rd = gzread(gzf_r, read_buf, (unsigned int)comp_len);
    int unget_ret = 0;
    if (gzf_r && gz_rd > 0) unget_ret = gzungetc((int)read_buf[0], gzf_r);
    if (gzf_r) {
        inf.next_in = comp_buf;
        inf.avail_in = (uInt)comp_len;
    }
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;
    uLong crc_val = crc32(0UL, decomp_buf, (uInt)decomp_len);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int gz_cl_r = 0;
    if (gzf_r) gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(decomp_buf);
    free(read_buf);
    (void)version;
    (void)init_def;
    (void)out_bound;
    (void)init_inf;
    (void)dict_set_ret;
    (void)def_ret;
    (void)pending;
    (void)pending_bits;
    (void)pend_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)unget_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)crc_val;
    (void)def_end;
    (void)inf_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}