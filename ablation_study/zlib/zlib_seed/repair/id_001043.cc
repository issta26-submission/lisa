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
//<ID> 1043
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence sample data for compress, write to gz, and read back.";
    uLong source_len = (uLong)(sizeof(source) - 1);

    // step 2: Configure (use deflateInit_ to obtain a safe bound for compression)
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    int def_end_ret = deflateEnd(&def_strm);
    (void)def_init_ret;
    (void)def_end_ret;

    // step 3: Operate (compress data, write header with gzputs, then write compressed bytes with gzwrite)
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, source, source_len, 6);
    uLong comp_size = (uLong)destLen;
    (void)comp_ret;
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzputs_ret = gzputs(gzw, "ZLIB-COMPRESSED-DATA\n");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    int gzclose_ret_w = gzclose(gzw);
    (void)gzputs_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret_w;

    // step 4: Validate / Cleanup (read back compressed bytes with gzfread and clean up)
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_size + 16));
    memset(read_buf, 0, (size_t)(comp_size + 16));
    z_size_t items_read = gzfread(read_buf, 1, (z_size_t)comp_size, gzr);
    int gzclose_ret_r = gzclose(gzr);
    (void)items_read;
    (void)gzclose_ret_r;
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}