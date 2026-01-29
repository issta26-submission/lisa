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
//<ID> 1191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib sequence payload for deflate/prime/write/read/ungetc usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));

    // step 2: Initialize and configure deflate stream, allocate output buffer
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_prime_ret = deflatePrime(&def_strm, 3, 5);

    // step 3: Operate deflate, finalize, persist compressed data, then read it back and use gzungetc + gzclose_r
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gw = gzopen("zlib_tmp_seq.gz", "wb");
    int gw_write_ret = gzwrite(gw, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size));
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen("zlib_tmp_seq.gz", "rb");
    unsigned int read_len = (unsigned int)((comp_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)comp_size);
    Bytef *read_buf = (Bytef *)malloc((size_t)(read_len + 16));
    memset(read_buf, 0, (int)(read_len + 16));
    int gzread_ret = gzread(gr, read_buf, read_len);
    int gzungetc_ret = gzungetc('X', gr);
    int gr_close_ret = gzclose_r(gr);

    // step 4: Validate and cleanup
    uLong crc = crc32(0UL, read_buf, (uInt)(gzread_ret > 0 ? gzread_ret : 0));
    int remove_ret = remove("zlib_tmp_seq.gz");
    free(comp_buf);
    free(read_buf);

    (void)version;
    (void)def_init_ret;
    (void)def_prime_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)comp_size;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)gzread_ret;
    (void)gzungetc_ret;
    (void)gr_close_ret;
    (void)crc;
    (void)remove_ret;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}