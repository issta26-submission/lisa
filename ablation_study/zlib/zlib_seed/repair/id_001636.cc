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
//<ID> 1636
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef source_data[] = "zlib API sequence test: deflate -> deflatePending -> deflateEnd -> inflate -> gzopen64 -> gzwrite";
    uLong source_len = (uLong)(sizeof(source_data) - 1);
    const char *ver = zlibVersion();

    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure
    uLong comp_bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_bound);
    memset(comp_buf, 0, (int)comp_bound);
    def_strm.next_in = (Bytef *)source_data;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)comp_bound;

    // step 3: Operate
    int def_ret = deflate(&def_strm, 4);
    unsigned int pending_bytes = 0;
    int pending_bits = 0;
    int pending_ret = deflatePending(&def_strm, &pending_bytes, &pending_bits);
    uLong compressed_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(source_len + 1));
    memset(decomp_buf, 0, (int)(source_len + 1));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)compressed_len;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)source_len;
    int inf_ret = inflate(&inf_strm, 4);
    uLong decompressed_len = inf_strm.total_out;
    int inf_end_ret = inflateEnd(&inf_strm);

    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzf, decomp_buf, (unsigned int)decompressed_len);
    int gz_close_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    free(comp_buf);
    free(decomp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)comp_bound;
    (void)def_ret;
    (void)pending_bytes;
    (void)pending_bits;
    (void)pending_ret;
    (void)compressed_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzf;
    (void)gw_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}