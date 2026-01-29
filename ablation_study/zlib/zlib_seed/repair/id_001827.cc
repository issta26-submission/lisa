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
//<ID> 1827
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate, deflateCopy, write gz via gzputs, clearerr, cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    z_stream copy_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure & Compress
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;

    // step 3: Copy deflate state & compute checksum
    int def_copy_ret = deflateCopy(&copy_strm, &def_strm);
    uLong check_adler = adler32(0UL, src, (uInt)src_len);
    char info_buf[128];
    memset(info_buf, 0, (int)sizeof(info_buf));
    snprintf(info_buf, sizeof(info_buf), "comp_len=%lu adler=%lu def_ret=%d def_copy_ret=%d", (unsigned long)comp_len, (unsigned long)check_adler, def_ret, def_copy_ret);

    // step 4: Operate (write info to gz), clear errors & Cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, info_buf);
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);
    int def_end_copy_ret = deflateEnd(&copy_strm);
    int def_end_ret = deflateEnd(&def_strm);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)check_adler;
    (void)gzputs_ret;
    (void)gzclose_ret;
    (void)def_end_copy_ret;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}