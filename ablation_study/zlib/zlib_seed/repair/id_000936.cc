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
//<ID> 936
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for deflate, write to .gz, reopen and read one byte.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int params_ret = deflateParams(&strm, 9, 0);
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLongf comp_len = (uLongf)((uLong)bound - (uLong)strm.avail_out);
    int def_end_ret = deflateEnd(&strm);
    gzFile gzf_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gz_close_w_ret = gzclose_w(gzf_w);

    // step 4: Validate / Cleanup
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_byte = gzgetc_(gzf_r);
    int gz_close_r_ret = gzclose(gzf_r);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)params_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)gz_wr;
    (void)gz_close_w_ret;
    (void)first_byte;
    (void)gz_close_r_ret;
    // API sequence test completed successfully
    return 66;
}