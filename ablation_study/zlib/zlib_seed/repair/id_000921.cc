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
//<ID> 921
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence combined test payload for deflate/compress2/inflate and gzwrite.";
    uLong src_len = (uLong)(sizeof(source) - 1);

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)src_len;
    uLong def_bound = deflateBound(&def_strm, src_len);
    Bytef *def_comp_buf = (Bytef *)malloc((size_t)def_bound);
    memset(def_comp_buf, 0, (size_t)def_bound);
    def_strm.next_out = def_comp_buf;
    def_strm.avail_out = (uInt)def_bound;

    // step 2: Configure
    int def_ret = deflate(&def_strm, 4);
    uLong def_comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    uLongf comp2_len = compressBound(src_len);
    Bytef *comp2_buf = (Bytef *)malloc((size_t)comp2_len);
    memset(comp2_buf, 0, (size_t)comp2_len);
    int comp2_ret = compress2(comp2_buf, &comp2_len, source, src_len, 6);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp2_buf;
    inf_strm.avail_in = (uInt)comp2_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 32));
    memset(decomp_buf, 0, (size_t)(src_len + 32));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 32);

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 4);

    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, source, (unsigned int)src_len);
    int gz_cl = gzclose(gzf);

    // step 4: Validate / Cleanup
    int inf_end = inflateEnd(&inf_strm);
    free(def_comp_buf);
    free(comp2_buf);
    free(decomp_buf);

    (void)version;
    (void)def_init;
    (void)def_bound;
    (void)def_ret;
    (void)def_comp_len;
    (void)def_end_ret;
    (void)comp2_ret;
    (void)inf_init;
    (void)inf_ret;
    (void)inf_end;
    (void)gz_wr;
    (void)gz_cl;

    // API sequence test completed successfully
    return 66;
}