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
//<ID> 775
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    Bytef plain[512];
    memset(plain, 'A', sizeof(plain));
    uLong plain_len = (uLong)sizeof(plain);

    uLong comp_bound = compressBound(plain_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp2_ret = compress2(comp_buf, &comp_len, plain, plain_len, 6);

    // step 2: Configure
    z_stream def;
    memset(&def, 0, sizeof(def));
    int def_init_ret = deflateInit_(&def, 6, version, (int)sizeof(z_stream));
    def.next_in = plain;
    def.avail_in = (uInt)plain_len;
    uLong def_out_bound = deflateBound(&def, plain_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_out_bound);
    memset(def_out, 0, (size_t)def_out_bound);
    def.next_out = def_out;
    def.avail_out = (uInt)def_out_bound;

    z_stream def_copy;
    memset(&def_copy, 0, sizeof(def_copy));

    z_stream back;
    memset(&back, 0, sizeof(back));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int infl_back_init_ret = inflateBackInit_(&back, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate
    int def_ret = deflate(&def, 4);
    int copy_ret = deflateCopy(&def_copy, &def);
    const char *fname = "test_zlib_api_sequence_tmp.gz";
    gzFile gzf = gzopen64(fname, "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_close_ret = gzclose(gzf);
    int infl_back_end_ret = inflateBackEnd(&back);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def);
    int def_copy_end_ret = deflateEnd(&def_copy);
    free(comp_buf);
    free(def_out);

    (void)version;
    (void)plain_len;
    (void)comp_bound;
    (void)comp2_ret;
    (void)def_init_ret;
    (void)def_out_bound;
    (void)def_ret;
    (void)copy_ret;
    (void)gz_wr;
    (void)gz_close_ret;
    (void)infl_back_init_ret;
    (void)infl_back_end_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    // API sequence test completed successfully
    return 66;
}