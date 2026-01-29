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
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int def_params_ret = deflateParams(&def_strm, 9, 0);
    const Bytef source[] = "Zlib API sequence payload for compression and gz file IO.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_close_w_ret = gzclose_w(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int gz_close_r_ret = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)def_params_ret;
    (void)bound;
    (void)comp_ret;
    (void)def_end_ret;
    (void)gz_wr;
    (void)gz_close_w_ret;
    (void)first_char;
    (void)gz_close_r_ret;
    // API sequence test completed successfully
    return 66;
}