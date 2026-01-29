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
//<ID> 1817
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Compress with deflateInit2_
    const Bytef src[] = "zlib API sequence test: deflateInit2_ -> deflate -> write gz -> inflate -> validate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *ver = zlibVersion();
    int def_init2_ret = deflateInit2_(&def_strm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = (uInt)src_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 2: Operate - write original data to a gz file, rewind and read it back
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, src, (unsigned int)src_len);
    int gzrewind_ret = gzrewind(gzf);
    Bytef *gz_buf = (Bytef *)malloc((size_t)(src_len + 128));
    memset(gz_buf, 0, (int)(src_len + 128));
    int gzread_ret = gzread(gzf, gz_buf, (unsigned int)(src_len + 128));
    int gzclose_ret = gzclose(gzf);

    // step 3: Initialize inflate, inflate compressed buffer and check sync point
    z_stream inf_strm;
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int inf_init_ret = inflateInit_(&inf_strm, ver, (int)sizeof(z_stream));
    Bytef *inf_out = (Bytef *)malloc((size_t)(src_len + 256));
    memset(inf_out, 0, (int)(src_len + 256));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_len > 0 ? (uInt)comp_len : 0U);
    inf_strm.next_out = inf_out;
    inf_strm.avail_out = (uInt)(src_len + 256);
    int inf_ret = inflate(&inf_strm, 0);
    int sync_point_ret = inflateSyncPoint(&inf_strm);
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: Validate & Cleanup
    uLong check_adler = adler32(0UL, gz_buf, (uInt)(gzread_ret > 0 ? (uInt)gzread_ret : 0U));
    free(comp_buf);
    free(gz_buf);
    free(inf_out);
    (void)def_init2_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzrewind_ret;
    (void)gzread_ret;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)sync_point_ret;
    (void)inf_end_ret;
    (void)check_adler;
    // API sequence test completed successfully
    return 66;
}