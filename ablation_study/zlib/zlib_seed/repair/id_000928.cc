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
//<ID> 928
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence test payload for compress2 -> inflate flow.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);

    // step 2: Configure
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int init_inf = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    uInt decomp_size = (uInt)(src_len + 64);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_size);
    memset(decomp_buf, 0, (size_t)decomp_size);
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = decomp_size;

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int init_def = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 3: Operate
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, source, (unsigned int)src_len);
    int gz_cl = gzclose(gzf);
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end = inflateEnd(&inf_strm);
    int def_end = deflateEnd(&def_strm);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)comp_ret;
    (void)init_inf;
    (void)decomp_size;
    (void)init_def;
    (void)gz_wr;
    (void)gz_cl;
    (void)inf_ret;
    (void)inf_end;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}