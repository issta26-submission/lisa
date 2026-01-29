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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream inflate_strm;
    z_stream deflate_strm;
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    gzFile gzf;
    z_size_t items_read;
    off_t seek_ret;
    int ret_inflate_init;
    int ret_deflate_init;
    int ret_gzbuffer;
    int ret_inflate;
    int ret_deflate_reset;
    int ret_inflate_end;
    int ret_deflate_end;

    // step 2: Setup
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    ret_deflate_init = deflateInit_(&deflate_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&inflate_strm, zlibVersion(), (int)sizeof(z_stream));
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    ret_gzbuffer = gzbuffer(gzf, 4096);

    // step 3: Core operations
    items_read = gzfread((voidp)comp_buf, (z_size_t)1, (z_size_t)sizeof(comp_buf), gzf);
    inflate_strm.next_in = comp_buf;
    inflate_strm.avail_in = (uInt)items_read;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inflate_strm, 0);
    seek_ret = gzseek(gzf, (off_t)0, 0);
    deflate_strm.next_in = decomp_buf;
    deflate_strm.avail_in = (uInt)inflate_strm.total_out;
    ret_deflate_reset = deflateReset(&deflate_strm);

    // step 4: Cleanup
    ret_inflate_end = inflateEnd(&inflate_strm);
    ret_deflate_end = deflateEnd(&deflate_strm);
    gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_gzbuffer;
    (void)items_read;
    (void)ret_inflate;
    (void)seek_ret;
    (void)ret_deflate_reset;
    (void)ret_inflate_end;
    (void)ret_deflate_end;

    // API sequence test completed successfully
    return 66;
}