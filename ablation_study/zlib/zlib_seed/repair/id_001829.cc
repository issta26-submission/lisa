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
//<ID> 1829
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "zlib API sequence test: deflate -> deflateCopy -> gzputs -> validate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    z_stream src_strm;
    z_stream copy_strm;
    memset(&src_strm, 0, (int)sizeof(src_strm));
    memset(&copy_strm, 0, (int)sizeof(copy_strm));
    int init_ret = deflateInit_(&src_strm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&src_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure & Compress
    src_strm.next_in = (Bytef *)src;
    src_strm.avail_in = (uInt)src_len;
    src_strm.next_out = comp_buf;
    src_strm.avail_out = (uInt)bound;
    int def_ret = deflate(&src_strm, 4);
    uLong comp_len = src_strm.total_out;
    int copy_ret = deflateCopy(&copy_strm, &src_strm);

    // step 3: Operate (write text to gz and manage error state)
    gzFile gzf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gzputs_ret = gzputs(gzf, "This line was written with gzputs()\n");
    gzclearerr(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate & Cleanup
    uLong src_adler = adler32(0UL, src, (uInt)src_len);
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)(comp_len > 0 ? comp_len : 0UL));
    int end_copy_ret = deflateEnd(&copy_strm);
    int end_src_ret = deflateEnd(&src_strm);
    free(comp_buf);
    (void)ver;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)copy_ret;
    (void)gzputs_ret;
    (void)gzclose_ret;
    (void)src_adler;
    (void)comp_crc;
    (void)end_copy_ret;
    (void)end_src_ret;
    // API sequence test completed successfully
    return 66;
}