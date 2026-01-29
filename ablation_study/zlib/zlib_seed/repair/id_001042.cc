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
//<ID> 1042
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Example data to compress with deflate and store in a gz file for readback.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 0);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;

    // step 2: Configure / Operate (write gz)
    gzFile gzw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int puts_ret = gzputs(gzw, "gz-header-line\n");
    int gzwrite_ret = gzwrite(gzw, comp_buf, (unsigned int)comp_size);
    int gzclose_ret_w = gzclose(gzw);
    (void)puts_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret_w;

    // step 3: Operate (read back with gzfread)
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    uLong out_buf_len = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (size_t)out_buf_len);
    z_size_t items_read = gzfread(out_buf, (z_size_t)1, (z_size_t)out_buf_len, gzr);
    int gzclose_ret_r = gzclose(gzr);
    (void)items_read;
    (void)gzclose_ret_r;

    // step 4: Validate / Cleanup
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)source_len;
    (void)bound;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}