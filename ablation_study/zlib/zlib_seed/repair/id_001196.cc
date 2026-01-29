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
//<ID> 1196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload demonstrating deflateInit_/deflatePrime/deflate/gzwrite/gzopen/gzungetc/gzclose_r usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate, prime bits, and perform compression
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_prime_ret = deflatePrime(&def_strm, 3, 5);
    int def_ret = deflate(&def_strm, 4); /* Z_FINISH == 4 */
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Persist original data into a gzip file (gzwrite will compress) and then open it for reading
    const char *fname = "zlib_test_tmp.gz";
    gzFile gw = gzopen(fname, "wb");
    int gw_write_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen(fname, "rb");
    char read_buf_stack[128];
    memset(read_buf_stack, 0, (int)sizeof(read_buf_stack));
    int gzungetc_ret = gzungetc((int)source[0], gr);
    int gzread_ret = gzread(gr, read_buf_stack, (unsigned int)((source_len < (uLong)sizeof(read_buf_stack)) ? (uInt)source_len : (uInt)sizeof(read_buf_stack)));

    // step 4: Cleanup resources, close read handle using gzclose_r, remove temporary file, free buffers
    int gr_close_r_ret = gzclose_r(gr);
    int remove_ret = remove(fname);
    free(comp_buf);

    (void)version;
    (void)def_init_ret;
    (void)def_prime_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)comp_size;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)gzungetc_ret;
    (void)gzread_ret;
    (void)gr_close_r_ret;
    (void)remove_ret;
    (void)read_buf_stack;
    // API sequence test completed successfully
    return 66;
}