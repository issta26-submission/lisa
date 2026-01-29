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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream instrm;
    Bytef input[] = "Hello zlib test data.";
    uInt input_len = (uInt)(sizeof(input) - 1);
    Bytef comp_buf[512];
    Bytef read_buf[512];
    Bytef decomp_buf[512];
    gzFile gzf;
    int ret_deflate_init;
    int ret_inflate_init;
    int ret_deflate;
    int ret_deflate_reset;
    int ret_deflate_end;
    int ret_inflate_end;
    int ret_inflate;
    int ret_gzbuffer;
    int ret_gzwrite;
    z_size_t items_read;
    uLong compressed_len;
    off_t seek_ret;

    // step 2: Setup
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&instrm, 0, sizeof(instrm));
    ret_deflate_init = deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)input;
    defstrm.avail_in = input_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)sizeof(comp_buf);
    ret_inflate_init = inflateInit_(&instrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Core operations
    ret_deflate = deflate(&defstrm, 0);
    compressed_len = defstrm.total_out;
    gzf = gzopen("test_zlib_api_sequence.tmp.gz", "w+b");
    ret_gzbuffer = gzbuffer(gzf, 4096);
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)compressed_len);
    seek_ret = gzseek(gzf, (off_t)0, 0);
    items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)compressed_len, gzf);
    instrm.next_in = read_buf;
    instrm.avail_in = (uInt)items_read;
    instrm.next_out = decomp_buf;
    instrm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&instrm, 0);
    ret_deflate_reset = deflateReset(&defstrm);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&defstrm);
    ret_inflate_end = inflateEnd(&instrm);
    gzclose(gzf);

    (void)ret_deflate_init; (void)ret_inflate_init; (void)ret_deflate; (void)ret_deflate_reset;
    (void)ret_deflate_end; (void)ret_inflate_end; (void)ret_inflate; (void)ret_gzbuffer;
    (void)ret_gzwrite; (void)items_read; (void)compressed_len; (void)seek_ret;

    // API sequence test completed successfully
    return 66;
}