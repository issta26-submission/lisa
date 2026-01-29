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
//<ID> 1192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib API sequence payload demonstrating deflateInit_/deflatePrime/gzungetc/gzclose_r usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);

    // step 2: Configure deflate, prime bitstream, compress to in-memory buffer, and finalize deflate
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int prime_ret = deflatePrime(&def_strm, 1, 0);
    int def_ret = deflate(&def_strm, 4); /* 4 used as flush (e.g., Z_FINISH) */
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Persist compressed data to a gz file, then open for reading, push back a byte, read, and close for read
    gzFile gw = gzopen("zlib_test_tmp.gz", "wb");
    int gw_write_ret = gzwrite(gw, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    int gw_close_ret = gzclose(gw);
    FILE *fr = fopen("zlib_test_tmp.gz", "rb");
    int fd = fileno(fr);
    int fd_dup = dup(fd);
    int fclose_fr_ret = fclose(fr);
    gzFile gr = gzdopen(fd_dup, "rb");
    /* push back the first byte of the compressed buffer (safe even if comp_size is 0 because buffer is allocated) */
    int push_byte = (int)(comp_buf[0] & 0xFF);
    int gzungetc_ret = gzungetc(push_byte, gr);
    unsigned int read_len = (unsigned int)((comp_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)comp_size);
    Bytef *read_buf = (Bytef *)malloc((size_t)(read_len + 16));
    memset(read_buf, 0, (int)(read_len + 16));
    int gzread_ret = gzread(gr, read_buf, read_len);
    int gr_close_ret = gzclose_r(gr);

    // step 4: Cleanup and finalization
    int remove_ret = remove("zlib_test_tmp.gz");
    free(comp_buf);
    free(read_buf);

    (void)version;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)fd;
    (void)fd_dup;
    (void)fclose_fr_ret;
    (void)gzungetc_ret;
    (void)gzread_ret;
    (void)gr_close_ret;
    (void)remove_ret;
    (void)source_len;
    (void)comp_size;
    (void)&def_strm;
    // API sequence test completed successfully
    return 66;
}