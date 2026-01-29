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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_strm;
    z_stream inflate_strm;
    Bytef input_data[] = "Example data to compress and then inflate using zlib APIs.";
    uInt input_len = (uInt)(sizeof(input_data) - 1);
    Bytef comp_buf[512];
    Bytef read_buf[512];
    Bytef decomp_buf[512];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_deflate_reset;
    int ret_inflate;
    int ret_deflate_end;
    int ret_inflate_end;
    int ret_gzbuffer;
    int ret_gzclose;
    int bytes_written;
    off_t seek_ret;
    z_size_t items_read;
    uLong comp_len;

    // step 2: Setup (initialize streams)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    ret_deflate_init = deflateInit_(&deflate_strm, 6, zlibVersion(), (int)sizeof(z_stream));
    ret_inflate_init = inflateInit_(&inflate_strm, zlibVersion(), (int)sizeof(z_stream));
    deflate_strm.next_in = (Bytef *)input_data;
    deflate_strm.avail_in = input_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Core operations (deflate, reset, write/read gz, inflate)
    ret_deflate = deflate(&deflate_strm, 0);
    comp_len = deflate_strm.total_out;
    ret_deflate_reset = deflateReset(&deflate_strm);
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "w+b");
    bytes_written = gzwrite(gzf, comp_buf, (unsigned int)comp_len);
    ret_gzbuffer = gzbuffer(gzf, 4096);
    seek_ret = gzseek(gzf, 0, 0);
    items_read = gzfread(read_buf, 1, (z_size_t)comp_len, gzf);
    inflate_strm.next_in = read_buf;
    inflate_strm.avail_in = (uInt)items_read;
    inflate_strm.next_out = decomp_buf;
    inflate_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inflate_strm, 0);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&deflate_strm);
    ret_inflate_end = inflateEnd(&inflate_strm);
    ret_gzclose = gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_inflate_init;
    (void)ret_deflate;
    (void)ret_deflate_reset;
    (void)bytes_written;
    (void)ret_gzbuffer;
    (void)seek_ret;
    (void)items_read;
    (void)ret_inflate;
    (void)ret_deflate_end;
    (void)ret_inflate_end;
    (void)ret_gzclose;
    (void)comp_len;

    // API sequence test completed successfully
    return 66;
}