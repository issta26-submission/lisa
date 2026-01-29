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
//<ID> 849
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example data for zlib API sequence with dictionary";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    int def_dict_ret = deflateSetDictionary(&def, dict, dict_len);
    int inf_dict_ret = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    int def_ret = deflate(&def, 4);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pend_ret = deflatePending(&def, &pending_bytes, &pending_bits);
    uLong comp_len = def.total_out;
    const char *fname = "zlib_test_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(readbuf, 0, (size_t)(comp_len + 16));
    int gz_rd = gzread(gzf_r, readbuf, (unsigned int)comp_len);
    int pushed = gzungetc((int)readbuf[0], gzf_r);
    int c_again = gzgetc(gzf_r);

    inf.next_in = comp_buf;
    inf.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(sample_len + 64));
    memset(decomp_buf, 0, (size_t)(sample_len + 64));
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)(sample_len + 64);
    int inf_ret = inflate(&inf, 4);
    uLong decomp_len = inf.total_out;
    uLong crc_val = crc32(0UL, decomp_buf, (uInt)decomp_len);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&def);
    int inf_end = inflateEnd(&inf);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(readbuf);
    free(decomp_buf);
    (void)version;
    (void)def_init;
    (void)inf_init;
    (void)def_dict_ret;
    (void)inf_dict_ret;
    (void)def_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)pend_ret;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)pushed;
    (void)c_again;
    (void)inf_ret;
    (void)decomp_len;
    (void)crc_val;
    (void)def_end;
    (void)inf_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}