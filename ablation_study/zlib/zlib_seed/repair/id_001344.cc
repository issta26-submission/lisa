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
//<ID> 1344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source and deflate stream)
    const Bytef src[] = "zlib api sequence: deflate -> write gz -> seek/read";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);

    // step 2: Configure (allocate output buffer based on bound)
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)bound);
    memset(out_buf, 0, (int)bound);
    def_strm.next_out = out_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate (deflate, finish, write compressed bytes to gz file, then read them back)
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = (uLong)def_strm.total_out;
    int end_ret = deflateEnd(&def_strm);
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_written = gzwrite(gw, out_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_ret = gzseek(gr, (off_t)0, 0);
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len ? comp_len : 1));
    memset(read_buf, 0, (int)(comp_len ? comp_len : 1));
    int gr_read = gzread(gr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int eof_flag = gzeof(gr);
    int gr_close_ret = gzclose(gr);

    // step 4: Validate & Cleanup
    free(out_buf);
    free(read_buf);
    (void)version;
    (void)init_ret;
    (void)def_ret;
    (void)end_ret;
    (void)gw_written;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)gr_read;
    (void)eof_flag;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}