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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_strm;
    z_stream deflate_copy_strm;
    z_stream inflate_strm;
    Bytef input[] = "Test data for zlib deflate and inflate sequence.";
    uInt input_len = (uInt)(sizeof(input) - 1);
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_inflate;
    int ret_deflate_copy;
    int ret_deflate_end_src;
    int ret_deflate_end_copy;
    int ret_inflate_end;
    int ret_deflate_tune;
    unsigned long codes_used;
    uLong comp_total;
    off_t offset;

    // step 2: Setup (initialize streams and configure)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&deflate_copy_strm, 0, sizeof(deflate_copy_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ret_deflate_init = deflateInit_(&deflate_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit2_(&inflate_strm, 15, zlibVersion(), (int)sizeof(z_stream));
    deflate_strm.next_in = input;
    deflate_strm.avail_in = input_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);
    ret_deflate_tune = deflateTune(&deflate_strm, 10, 20, 30, 40);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    gzbuffer(gzf, 4096);

    // step 3: Core operations (compress, copy state, decompress, validate)
    ret_deflate = deflate(&deflate_strm, 0);
    comp_total = deflate_strm.total_out;
    gzwrite(gzf, comp_buf, (unsigned int)comp_total);
    ret_deflate_copy = deflateCopy(&deflate_copy_strm, &deflate_strm);
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)comp_total;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inflate_strm, 0);
    codes_used = inflateCodesUsed(&inflate_strm);
    offset = gzoffset(gzf);

    // step 4: Cleanup (end streams and close file)
    ret_deflate_end_src = deflateEnd(&deflate_strm);
    ret_deflate_end_copy = deflateEnd(&deflate_copy_strm);
    ret_inflate_end = inflateEnd(&inflate_strm);
    gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate;
    (void)ret_inflate;
    (void)ret_deflate_copy;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_copy;
    (void)ret_inflate_end;
    (void)ret_deflate_tune;
    (void)codes_used;
    (void)comp_total;
    (void)offset;

    // API sequence test completed successfully
    return 66;
}