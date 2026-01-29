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
//<ID> 929
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Zlib API sequence payload to compress, write to gz, and then inflate back.";
    uLong src_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure (compression buffer allocation and compress)
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, src_len, 6);

    // step 3: Operate (finalize deflate stream, write compressed data to .gz, then inflate)
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gzf = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    int gz_cl = gzclose(gzf);
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)comp_len;
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(decomp_buf, 0, (size_t)(src_len + 64));
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)(src_len + 64);
    int inf_ret = inflate(&inf_strm, 4);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(decomp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)comp_ret;
    (void)def_end_ret;
    (void)gz_wr;
    (void)gz_cl;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}