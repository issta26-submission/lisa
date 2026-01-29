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
//<ID> 1877
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzopen/gzwrite -> gzungetc -> inflateMark";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compressor and buffers
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate - compress and write to gzip file, then push a byte back
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    gzFile gzf = gzopen("test_output.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)(comp_len > 0 ? (unsigned int)comp_len : 0U));
    int unget_ret = gzungetc('A', gzf);

    // step 4: Validate & Cleanup
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    long mark = inflateMark(&infstrm);
    int def_end_ret = deflateEnd(&strm);
    int inf_end_ret = inflateEnd(&infstrm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)gz_wr;
    (void)unget_ret;
    (void)inf_init_ret;
    (void)mark;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gz_close_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}