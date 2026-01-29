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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_strm;
    z_stream copy_strm;
    z_stream inflate_strm;
    Bytef input[] = "The quick brown fox jumps over the lazy dog";
    uInt input_len = (uInt)(sizeof(input) - 1);
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    off_t offset;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_inflate;
    int ret_deflatecopy;
    int ret_deflate_tune;
    int ret_deflate_end_def;
    int ret_deflate_end_copy;
    int ret_inflate_end;
    unsigned long codes_used;
    unsigned int compressed_len;
    const char * ver;

    // step 2: Setup
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&copy_strm, 0, sizeof(copy_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));
    ret_inflate_init = inflateInit2_(&inflate_strm, 15, ver, (int)sizeof(z_stream));
    deflate_strm.next_in = input;
    deflate_strm.avail_in = input_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations
    ret_deflate = deflate(&deflate_strm, 0);
    compressed_len = (unsigned int)deflate_strm.total_out;
    gzwrite(gzf, comp_buf, compressed_len);
    ret_deflatecopy = deflateCopy(&copy_strm, &deflate_strm);
    ret_deflate_tune = deflateTune(&deflate_strm, 4, 4, 8, 16);
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)compressed_len;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inflate_strm, 0);
    codes_used = inflateCodesUsed(&inflate_strm);
    offset = gzoffset(gzf);

    // step 4: Cleanup
    ret_deflate_end_def = deflateEnd(&deflate_strm);
    ret_deflate_end_copy = deflateEnd(&copy_strm);
    ret_inflate_end = inflateEnd(&inflate_strm);
    gzclose(gzf);
    (void)ver;
    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate;
    (void)compressed_len;
    (void)ret_deflatecopy;
    (void)ret_deflate_tune;
    (void)ret_inflate;
    (void)codes_used;
    (void)offset;
    (void)ret_deflate_end_def;
    (void)ret_deflate_end_copy;
    (void)ret_inflate_end;

    // API sequence test completed successfully
    return 66;
}