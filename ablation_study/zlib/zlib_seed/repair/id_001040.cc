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
//<ID> 1040
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence: initialize deflate, write via gzputs/gzwrite, read via gzfread.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    uLong bound = deflateBound(&defstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 3: Operate (write then read gz file)
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int put_ret = gzputs(gzw, "HEADER: zlib sequence\n");
    int write_ret = gzwrite(gzw, (voidpc)source, (unsigned int)source_len);
    int close_w_ret = gzclose(gzw);
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    z_size_t read_buf_len = (z_size_t)(source_len + 32);
    voidp read_buf = malloc((size_t)read_buf_len);
    memset(read_buf, 0, (size_t)read_buf_len);
    z_size_t items_read = gzfread(read_buf, 1, read_buf_len, gzr);
    int close_r_ret = gzclose(gzr);

    // step 4: Validate / Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    free(comp_buf);
    free(read_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)put_ret;
    (void)write_ret;
    (void)close_w_ret;
    (void)items_read;
    (void)close_r_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}