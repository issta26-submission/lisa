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
//<ID> 847
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example data to compress with zlib and test APIs";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    uLong sample_crc = crc32(0L, sample, (uInt)sample_len);
    z_stream def;
    z_stream inf;
    memset(&def, 0, sizeof(def));
    memset(&inf, 0, sizeof(inf));
    int def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int inf_set_dict_ret = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    unsigned int pending_bits = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def, &pending_bits, &bits);
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    int gu_ret = gzungetc((int)comp_buf[0], gzf_r);
    Bytef *readbuf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(readbuf, 0, (size_t)(comp_len + 16));
    int gz_rd = gzread(gzf_r, readbuf, (unsigned int)comp_len);

    // step 4: Validate / Cleanup
    int gz_cl_r = gzclose(gzf_r);
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    free(comp_buf);
    free(readbuf);
    (void)version;
    (void)sample_crc;
    (void)def_init;
    (void)inf_init;
    (void)out_bound;
    (void)inf_set_dict_ret;
    (void)def_ret;
    (void)comp_len;
    (void)pending_bits;
    (void)bits;
    (void)pending_ret;
    (void)fname;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gu_ret;
    (void)gz_rd;
    (void)gz_cl_r;
    (void)def_end;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}