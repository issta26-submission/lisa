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
//<ID> 937
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for deflate then gz write/read.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int params_ret = deflateParams(&def_strm, 9, 0);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong out_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)out_len);
    int gzclose_w_ret = gzclose_w(gzf_w);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_byte = gzgetc_(gzf_r);
    int gzclose_r_ret = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)bound;
    (void)params_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gz_wr;
    (void)gzclose_w_ret;
    (void)first_byte;
    (void)gzclose_r_ret;
    // API sequence test completed successfully
    return 66;
}