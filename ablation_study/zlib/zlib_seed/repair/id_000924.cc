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
//<ID> 924
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for compress2 -> inflate -> gzwrite test.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong comp_bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (size_t)comp_bound);
    uLongf comp_len = (uLongf)comp_bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, source, (unsigned int)src_len);

    // step 3: Operate
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    int def_end_ret = deflateEnd(&def_strm);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    uLong decomp_bound = src_len + 16;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_bound);
    memset(decomp_buf, 0, (size_t)decomp_bound);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)decomp_bound;
    int inf_ret = inflate(&inf_strm, 4);

    // step 4: Validate / Cleanup
    int inf_end_ret = inflateEnd(&inf_strm);
    int gz_cl = gzclose(gzf);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)comp_bound;
    (void)comp_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)def_end_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gz_cl;
    // API sequence test completed successfully
    return 66;
}