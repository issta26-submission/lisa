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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream src;
    z_stream dst;
    z_stream inf;
    Bytef input_buf[] = "Example data for zlib compress/decompress sequence to exercise deflateCopy and inflateInit2_";
    uInt input_len = (uInt)(sizeof(input_buf) - 1);
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init2;
    int ret_deflate;
    int ret_inflate;
    int ret_deflatecopy;
    int ret_deflate_tune;
    int ret_deflate_end_src;
    int ret_deflate_end_dst;
    int ret_inflate_end;
    int ret_gzclose;
    off_t offset;
    unsigned long codes_used;
    unsigned int written;

    // step 2: Setup
    memset(&src, 0, sizeof(src));
    memset(&dst, 0, sizeof(dst));
    memset(&inf, 0, sizeof(inf));
    ret_deflate_init = deflateInit_(&src, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init2 = inflateInit2_(&inf, 15, zlibVersion(), (int)sizeof(z_stream));
    src.next_in = input_buf;
    src.avail_in = input_len;
    src.next_out = comp_buf;
    src.avail_out = (uInt)sizeof(comp_buf);
    inf.next_out = decomp_buf;
    inf.avail_out = (uInt)sizeof(decomp_buf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations
    ret_deflate = deflate(&src, 0);
    written = (unsigned int)src.total_out;
    if (gzf) { /* keep straight-line flow; gzwrite allowed even if pointer is null in this context */ }
    written = (unsigned int)gzwrite(gzf, comp_buf, written);
    offset = gzoffset(gzf);
    ret_deflatecopy = deflateCopy(&dst, &src);
    ret_deflate_tune = deflateTune(&dst, 32, 64, 128, 2048);
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)src.total_out;
    ret_inflate = inflate(&inf, 0);
    codes_used = inflateCodesUsed(&inf);

    // step 4: Cleanup
    ret_deflate_end_src = deflateEnd(&src);
    ret_deflate_end_dst = deflateEnd(&dst);
    ret_inflate_end = inflateEnd(&inf);
    ret_gzclose = gzclose(gzf);
    (void)ret_deflate_init;
    (void)ret_inflate_init2;
    (void)ret_deflate;
    (void)written;
    (void)offset;
    (void)ret_deflatecopy;
    (void)ret_deflate_tune;
    (void)ret_inflate;
    (void)codes_used;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_dst;
    (void)ret_inflate_end;
    (void)ret_gzclose;
    // API sequence test completed successfully
    return 66;
}