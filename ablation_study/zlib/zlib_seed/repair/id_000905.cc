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
//<ID> 905
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for compression->decompression validation.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)src_len;
    uLong out_bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_bound);
    memset(comp_buf, 0, (size_t)out_bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)out_bound;

    // step 3: Operate
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);
    z_stream instrm;
    memset(&instrm, 0, sizeof(instrm));
    int inf_init_ret = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)comp_len;
    uLong decomp_buf_len = src_len + 64;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_buf_len);
    memset(decomp_buf, 0, (size_t)decomp_buf_len);
    instrm.next_out = decomp_buf;
    instrm.avail_out = (uInt)decomp_buf_len;
    int inf_ret = inflate(&instrm, 4);
    uLong decomp_len = instrm.total_out;
    int inf_end_ret = inflateEnd(&instrm);

    // step 4: Validate / Cleanup
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, source, (unsigned int)src_len);
    int gz_cl = gzclose(gzf);
    gzFile gzf_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int first_char = gzgetc_(gzf_r);
    int gz_cl_r = gzclose(gzf_r);
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)out_bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_len;
    (void)inf_end_ret;
    (void)gz_wr;
    (void)gz_cl;
    (void)first_char;
    (void)gz_cl_r;
    // API sequence test completed successfully
    return 66;
}