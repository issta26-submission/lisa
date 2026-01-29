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
//<ID> 927
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compression and decompression.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong compress_bound = compressBound(src_len);
    Bytef *comp2_buf = (Bytef *)malloc((size_t)compress_bound);
    memset(comp2_buf, 0, (size_t)compress_bound);
    uLongf comp2_len = (uLongf)compress_bound;
    int comp2_ret = compress2(comp2_buf, &comp2_len, source, src_len, 6);

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));

    // step 2: Configure
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *def_out = (Bytef *)malloc((size_t)def_bound);
    memset(def_out, 0, (size_t)def_bound);
    def_strm.next_out = def_out;
    def_strm.avail_out = (uInt)def_bound;

    inf_strm.next_in = comp2_buf;
    inf_strm.avail_in = (uInt)comp2_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (size_t)(src_len + 16));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 16);

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    uLong deflated_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr_ret = gzwrite(gzf, source, (unsigned int)src_len);
    int gz_cl_ret = gzclose(gzf);

    int inf_ret = inflate(&inf_strm, 4);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate / Cleanup
    free(comp2_buf);
    free(def_out);
    free(decomp_buf);
    (void)version;
    (void)compress_bound;
    (void)comp2_len;
    (void)comp2_ret;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)def_ret;
    (void)deflated_len;
    (void)def_end_ret;
    (void)gz_wr_ret;
    (void)gz_cl_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}