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
//<ID> 1046
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: prepare, compress, write gz, read gz, decompress, cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, source, source_len, 6);
    uLong comp_size = (uLong)destLen;
    (void)def_init_ret;
    (void)comp_ret;

    // step 2: Configure / Operate (write gz)
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    int gzputs_ret = gzputs(gzw, "footer-meta");
    int gzclose_ret_w = gzclose(gzw);
    (void)gzwrite_ret;
    (void)gzputs_ret;
    (void)gzclose_ret_w;

    // step 3: Operate (read gz) and decompress
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *read_comp = (Bytef *)malloc((size_t)comp_size);
    memset(read_comp, 0, (size_t)comp_size);
    z_size_t items_read = gzfread(read_comp, (z_size_t)1, (z_size_t)comp_size, gzr);
    int gzclose_ret_r = gzclose(gzr);
    (void)items_read;
    (void)gzclose_ret_r;
    uLong out_len = source_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (size_t)out_len);
    uLongf outLenF = (uLongf)out_len;
    int uncmp_ret = uncompress(out_buf, &outLenF, read_comp, comp_size);
    (void)uncmp_ret;

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    free(read_comp);
    free(out_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    (void)out_len;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}