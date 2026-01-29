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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload for compress2, inflate and gzwrite testing.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);

    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, source, (unsigned int)src_len);

    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (size_t)(src_len + 16));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 16);

    // step 3: Operate
    int inf_ret = inflate(&inf_strm, 4);
    int def_end = deflateEnd(&def_strm);
    int gz_cl = gzclose(gzf);
    int inf_end = inflateEnd(&inf_strm);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)comp_ret;
    (void)def_init;
    (void)gz_wr;
    (void)inf_init;
    (void)inf_ret;
    (void)def_end;
    (void)gz_cl;
    (void)inf_end;
    // API sequence test completed successfully
    return 66;
}