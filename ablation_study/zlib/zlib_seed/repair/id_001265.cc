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
//<ID> 1265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef src[] = "zlib API sequence payload: compress, write header with gzputs, write compressed bytes, reopen and read a byte";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int def_init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure compression buffers and inputs
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate: deflate, write header with gzputs and compressed bytes, then reopen and inspect
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    int def_end_ret = deflateEnd(&strm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_puts_ret = gzputs(gw, "HEADER:");
    int gz_write_ret = gzwrite(gw, comp_buf, (unsigned int)(comp_size));
    int gzclose_w_ret = gzclose_w ? gzclose_w(gw) : gzclose(gw); /* use gzclose_w if available else gzclose */
    (void)gzclose_w_ret;

    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_pos = gzseek(gr, 0, 0);
    int first_char = gzgetc(gr);
    int gzclose_r_ret = gzclose_r ? gzclose_r(gr) : gzclose(gr); /* use gzclose_r if available else gzclose */
    (void)gzclose_r_ret;

    // step 4: Cleanup and validation
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_puts_ret;
    (void)gz_write_ret;
    (void)seek_pos;
    (void)first_char;
    // API sequence test completed successfully
    return 66;
}