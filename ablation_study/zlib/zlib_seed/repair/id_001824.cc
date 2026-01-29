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
//<ID> 1824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: initialize -> deflate -> deflateCopy -> write gz -> cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream def_strm;
    z_stream def_copy;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&def_copy, 0, (int)sizeof(def_copy));
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

    // step 3: Duplicate deflate state, write compressed data into a gz file, and clear errors
    int def_copy_ret = deflateCopy(&def_copy, &def_strm);
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, "zlib: compressed payload follows\n");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int def_copy_end_ret = deflateEnd(&def_copy);
    uLong check_adler = adler32(0UL, src, (uInt)src_len);
    free(comp_buf);
    (void)ver;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_copy_ret;
    (void)gzputs_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)def_end_ret;
    (void)def_copy_end_ret;
    (void)check_adler;
    // API sequence test completed successfully
    return 66;
}