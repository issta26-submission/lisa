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
//<ID> 939
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
    int init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int params_ret = deflateParams(&def_strm, 5, 0);
    const Bytef source[] = "Zlib API sequence payload for compression and gz write/read.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 5);

    // step 3: Operate
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_written = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gzclose_w_ret = gzclose_w(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)params_ret;
    (void)bound;
    (void)comp_ret;
    (void)def_end_ret;
    (void)gz_written;
    (void)gzclose_w_ret;
    (void)first_char;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}