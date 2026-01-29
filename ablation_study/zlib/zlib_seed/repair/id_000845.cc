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
//<ID> 845
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example data for zlib API sequence";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    z_stream inf;
    memset(&def, 0, sizeof(def));
    memset(&inf, 0, sizeof(inf));
    int init_def = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    int init_inf = inflateInit_(&inf, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int inf_set_dict = inflateSetDictionary(&inf, dict, dict_len);
    unsigned int pending = 0u;
    int pending_bits = 0;

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    int pend_ret = deflatePending(&def, &pending, &pending_bits);
    uLong crc_of_sample = crc32(0UL, sample, (uInt)sample_len);
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(readbuf, 0, (size_t)(comp_len + 16));
    int gz_rd = gzread(gzf_r, readbuf, (unsigned int)comp_len);
    int pushback_ret = gzungetc((int)readbuf[0], gzf_r);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(readbuf);
    (void)init_def;
    (void)init_inf;
    (void)out_bound;
    (void)inf_set_dict;
    (void)def_ret;
    (void)comp_len;
    (void)pend_ret;
    (void)pending;
    (void)pending_bits;
    (void)crc_of_sample;
    (void)fname;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)pushback_ret;
    (void)def_end;
    (void)inf_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}