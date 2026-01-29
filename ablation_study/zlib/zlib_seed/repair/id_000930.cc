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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for deflate, write to gz, and read first byte back.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int params_ret = deflateParams(&strm, 9, 0);
    uLong bound = deflateBound(&strm, src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)bound);
    memset(out_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)src_len;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    int def_end_ret = deflateEnd(&strm);
    uLong comp_len = strm.total_out;
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf_w, out_buf, (unsigned int)comp_len);
    int gz_close_w_ret = gzclose_w(gzf_w);

    // step 4: Validate / Cleanup
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_byte = gzgetc_(gzf_r);
    int gz_close_r_ret = gzclose(gzf_r);
    free(out_buf);
    (void)init_ret;
    (void)params_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)gz_wr;
    (void)gz_close_w_ret;
    (void)first_byte;
    (void)gz_close_r_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}