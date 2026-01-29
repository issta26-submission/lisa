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
//<ID> 69
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
    Bytef input_buf[] = "Example input data for zlib compression and inflate testing.";
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_copy;
    int ret_deflate_tune;
    int ret_deflate_end_src;
    int ret_deflate_end_copy;
    int ret_inflate_init;
    int ret_inflate;
    int ret_inflate_end;
    off_t file_offset;
    unsigned long codes_used;
    const char *ver;

    // step 2: Setup (initialize streams and open gz file)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&deflate_copy_strm, 0, sizeof(deflate_copy_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));
    deflate_strm.next_in = input_buf;
    deflate_strm.avail_in = (uInt)(sizeof(input_buf) - 1);
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (compress, write, copy/tune, then inflate and inspect)
    ret_deflate = deflate(&deflate_strm, 0);
    gzwrite(gzf, comp_buf, (unsigned int)deflate_strm.total_out);
    file_offset = gzoffset(gzf);
    ret_deflate_copy = deflateCopy(&deflate_copy_strm, &deflate_strm);
    ret_deflate_tune = deflateTune(&deflate_copy_strm, 4, 16, 64, 256);
    ret_inflate_init = inflateInit2_(&inflate_strm, 15, ver, (int)sizeof(z_stream));
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)deflate_strm.total_out;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inflate_strm, 0);
    codes_used = inflateCodesUsed(&inflate_strm);

    // step 4: Cleanup (end streams and close file)
    ret_deflate_end_src = deflateEnd(&deflate_strm);
    ret_deflate_end_copy = deflateEnd(&deflate_copy_strm);
    ret_inflate_end = inflateEnd(&inflate_strm);
    gzclose(gzf);

    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate;
    (void)ret_deflate_copy;
    (void)ret_deflate_tune;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)codes_used;
    (void)file_offset;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_copy;
    (void)ret_inflate_end;

    // API sequence test completed successfully
    return 66;
}