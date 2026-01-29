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
//<ID> 1197
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload demonstrating deflateInit_/deflatePrime/deflate/gzopen/gzwrite/gzungetc/gzread/gzclose_r usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure compression, prime bit buffer, and perform deflate
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int prime_ret = deflatePrime(&def_strm, 3, 5);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;

    // step 3: Persist compressed data to a gzip file, reopen for reading, push back a byte and read it back, then close read handle
    gzFile gw = gzopen("zlib_api_seq_tmp.gz", "wb");
    int gw_write_ret = gzwrite(gw, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen("zlib_api_seq_tmp.gz", "rb");
    int unget_ret = gzungetc('B', gr);
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_size + 16));
    memset(read_buf, 0, (int)(comp_size + 16));
    unsigned int to_read = (unsigned int)((comp_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)comp_size);
    int gzread_ret = gzread(gr, read_buf, to_read);
    int gr_close_r_ret = gzclose_r(gr);

    // step 4: Cleanup, finalize deflate, free buffers and remove temporary file
    int def_end_ret = deflateEnd(&def_strm);
    int remove_ret = remove("zlib_api_seq_tmp.gz");
    free(comp_buf);
    free(read_buf);

    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)prime_ret;
    (void)def_ret;
    (void)comp_size;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)unget_ret;
    (void)gzread_ret;
    (void)gr_close_r_ret;
    (void)def_end_ret;
    (void)remove_ret;
    // API sequence test completed successfully
    return 66;
}