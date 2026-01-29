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
//<ID> 844
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Sample data for dictionary usage in zlib";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "shared_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_def = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int setdict_def = deflateSetDictionary(&def, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    unsigned int pending_count = 0;
    int pending_bits = 0;
    int pend_ret = deflatePending(&def, &pending_count, &pending_bits);

    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int init_inf = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    uLong decomp_buf_size = sample_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_buf_size);
    memset(decomp_buf, 0, (size_t)decomp_buf_size);
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)decomp_buf_size;
    int setdict_inf = inflateSetDictionary(&inf, dict, dict_len);
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;

    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    int gz_unpush = gzungetc('X', gzf_r);
    Bytef *readbuf = (Bytef *)malloc((size_t)(decomp_len + 16));
    memset(readbuf, 0, (size_t)(decomp_len + 16));
    int gz_rd = gzread(gzf_r, readbuf, (unsigned int)(decomp_len + 16));
    int gz_c = gzgetc(gzf_r);
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    uLong crc_sample = crc32(0L, sample, (uInt)sample_len);
    uLong crc_decomp = crc32(0L, decomp_buf, (uInt)decomp_len);
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    free(comp_buf);
    free(decomp_buf);
    free(readbuf);
    (void)version;
    (void)init_def;
    (void)setdict_def;
    (void)def_ret;
    (void)comp_len;
    (void)pending_count;
    (void)pending_bits;
    (void)pend_ret;
    (void)init_inf;
    (void)setdict_inf;
    (void)inf_ret;
    (void)decomp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_unpush;
    (void)gz_rd;
    (void)gz_c;
    (void)gz_cl_r;
    (void)crc_sample;
    (void)crc_decomp;
    (void)def_end;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}