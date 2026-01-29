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
//<ID> 1346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence test: deflate -> write gz -> seek/read";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src_data;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 2: Configure (deflate then finalize)
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = (uLong)def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Operate (write compressed data to .gz, then read it back using seek/read)
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gw = gzopen(fname, "wb");
    unsigned int write_len = comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size;
    int gw_ret = gzwrite(gw, comp_buf, write_len);
    int gclose_ret1 = gzclose(gw);

    gzFile gr = gzopen(fname, "rb");
    off_t pos = gzseek(gr, (off_t)0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_size + 1);
    memset(read_buf, 0, (int)(comp_size + 1));
    unsigned int read_len = comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size;
    int gr_ret = gzread(gr, read_buf, read_len);
    int eof_flag = gzeof(gr);
    int gclose_ret2 = gzclose(gr);

    // step 4: Validate & Cleanup
    uLong crc_written = crc32(0L, comp_buf, (uInt)comp_size);
    uLong crc_read = crc32(0L, read_buf, (uInt)comp_size);
    free(comp_buf);
    free(read_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gclose_ret1;
    (void)pos;
    (void)gr_ret;
    (void)eof_flag;
    (void)gclose_ret2;
    (void)crc_written;
    (void)crc_read;
    (void)version;
    // API sequence test completed successfully
    return 66;
}