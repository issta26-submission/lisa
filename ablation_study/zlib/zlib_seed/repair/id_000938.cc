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
//<ID> 938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib deflate payload for gz write/read sequence.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    int def_param_ret = deflateParams(&def_strm, 6, 0);
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *outbuf = (Bytef *)malloc((size_t)bound);
    memset(outbuf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = outbuf;
    def_strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong produced = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, outbuf, (unsigned int)produced);
    int gz_close_w_ret = gzclose_w(gzf);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int gz_close_r_ret = gzclose(gzf_r);

    // step 4: Validate / Cleanup
    free(outbuf);
    (void)version;
    (void)def_init_ret;
    (void)def_param_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)gz_wr;
    (void)gz_close_w_ret;
    (void)first_char;
    (void)gz_close_r_ret;
    // API sequence test completed successfully
    return 66;
}