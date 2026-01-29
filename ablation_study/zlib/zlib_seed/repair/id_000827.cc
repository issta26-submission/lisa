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
//<ID> 827
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef input_data[] = "The quick brown fox jumps over the lazy dog";
    uLong input_len = (uLong)(sizeof(input_data) - 1);
    z_stream def;
    memset(&def, 0, sizeof(def));
    int init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    int reset_ret = deflateReset(&def);
    uLong out_bound = deflateBound(&def, input_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    def.next_in = (Bytef *)input_data;
    def.avail_in = (uInt)input_len;
    def.next_out = comp_buf;
    def.avail_out = (uInt)out_bound;

    // step 2: Configure
    int def_ret = deflate(&def, 4);
    uLong comp_len = def.total_out;
    z_stream back;
    memset(&back, 0, sizeof(back));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int ibinit_ret = inflateBackInit_(&back, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf_w = gzopen(fname, "wb");
    int gzset_ret = gzsetparams(gzf_w, 1, 0);
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    off_t pos_after_write = gztell(gzf_w);
    int gz_cl_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen(fname, "rb");
    Bytef *read_back = (Bytef *)malloc((size_t)out_bound);
    memset(read_back, 0, (size_t)out_bound);
    int gz_rd = gzread(gzf_r, read_back, (unsigned int)comp_len);
    off_t pos_after_readopen = gztell(gzf_r);
    int gz_cl_r = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    int inf_back_end = inflateBackEnd(&back);
    int def_end = deflateEnd(&def);
    free(comp_buf);
    free(read_back);
    (void)version;
    (void)init_ret;
    (void)reset_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)ibinit_ret;
    (void)fname;
    (void)gzset_ret;
    (void)gz_wr;
    (void)pos_after_write;
    (void)gz_cl_w;
    (void)gz_rd;
    (void)pos_after_readopen;
    (void)gz_cl_r;
    (void)inf_back_end;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}