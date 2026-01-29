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
//<ID> 1047
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib fuzz-driver: sample payload to compress, write with gz, and read back with gzfread.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound_est = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound_est);
    memset(comp_buf, 0, (size_t)bound_est);
    uLongf comp_buf_len = (uLongf)bound_est;

    // step 2: Configure
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong def_bound = deflateBound(&defstrm, source_len);
    (void)def_bound;

    // step 3: Operate
    int comp_ret = compress2(comp_buf, &comp_buf_len, source, source_len, 6);
    uLong comp_size = (uLong)comp_buf_len;
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    int gzputs_ret = gzputs(gzw, " --gz-meta--");
    int gzclose_w_ret = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    size_t read_buf_len = (size_t)(comp_size + 32);
    voidp read_buf = malloc(read_buf_len);
    memset(read_buf, 0, read_buf_len);
    z_size_t nread = gzfread(read_buf, (z_size_t)1, (z_size_t)read_buf_len, gzr);
    int gzclose_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)source_len;
    (void)bound_est;
    (void)def_init_ret;
    (void)comp_ret;
    (void)comp_size;
    (void)gzwrite_ret;
    (void)gzputs_ret;
    (void)gzclose_w_ret;
    (void)nread;
    (void)gzclose_r_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}