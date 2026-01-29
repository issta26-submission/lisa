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
//<ID> 842
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef sample[] = "Example sample data for zlib APIs";
    uLong sample_len = (uLong)(sizeof(sample) - 1);
    const Bytef dict[] = "example_dictionary";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = (Bytef *)sample;
    def.avail_in = (uInt)sample_len;
    uLong out_bound = deflateBound(&def, sample_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));

    // step 2: Configure
    int dict_set_ret = inflateSetDictionary(&inf, dict, dict_len);

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    unsigned int pending = 0;
    int bits = 0;
    int pending_ret = deflatePending(&def, &pending, &bits);
    uLong comp_len = def.total_out;
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    int unget_ret = gzungetc('A', gzf_r);
    unsigned int read_buf_size = (unsigned int)(comp_len + 64);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (size_t)read_buf_size);
    int gz_rd = gzread(gzf_r, read_buf, read_buf_size);
    int gotc = gzgetc(gzf_r);
    inf.next_in = read_buf;
    inf.avail_in = (uInt)gz_rd;
    Bytef *decomp_out = (Bytef *)malloc((size_t)(sample_len + 128));
    memset(decomp_out, 0, (size_t)(sample_len + 128));
    inf.next_out = decomp_out;
    inf.avail_out = (uInt)(sample_len + 128);
    int inf_ret = inflate(&inf, 4);
    uLong crc = crc32(0L, sample, (uInt)sample_len);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&inf);
    int def_end = deflateEnd(&def);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(read_buf);
    free(decomp_out);
    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)inf_init_ret;
    (void)dict_set_ret;
    (void)def_ret;
    (void)pending_ret;
    (void)pending;
    (void)bits;
    (void)comp_len;
    (void)fname;
    (void)gz_wr;
    (void)gz_cl_w;
    (void)unget_ret;
    (void)gz_rd;
    (void)gotc;
    (void)inf_ret;
    (void)crc;
    (void)inf_end;
    (void)def_end;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}