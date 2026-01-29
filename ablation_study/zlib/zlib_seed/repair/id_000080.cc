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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream def_strm;
    z_stream def_copy;
    z_stream inf_strm;
    Bytef input_buf[] = "Example input data for zlib compression and inflate testing.";
    Bytef comp_buf[1024];
    Bytef decomp_buf[1024];
    const char *ver;
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
    int ret_gzwrite;

    // step 2: Setup
    memset(&def_strm, 0, sizeof(def_strm));
    memset(&def_copy, 0, sizeof(def_copy));
    memset(&inf_strm, 0, sizeof(inf_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit2_(&def_strm, 6, 8, 15, 8, 0, ver, (int)sizeof(z_stream));
    def_strm.next_in = input_buf;
    def_strm.avail_in = (uInt)(sizeof(input_buf) - 1);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Core operations (compress, copy, tune, write, then inflate)
    ret_deflate = deflate(&def_strm, 0);
    ret_deflate_copy = deflateCopy(&def_copy, &def_strm);
    ret_deflate_tune = deflateTune(&def_copy, 4, 16, 32, 128);
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)def_strm.total_out);
    ret_inflate_init = inflateInit2_(&inf_strm, 15, ver, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)def_strm.total_out;
    inf_strm.next_out = decomp_buf;
    inf_strm.avail_out = (uInt)sizeof(decomp_buf);
    ret_inflate = inflate(&inf_strm, 0);

    // step 4: Cleanup
    ret_deflate_end_src = deflateEnd(&def_strm);
    ret_deflate_end_copy = deflateEnd(&def_copy);
    ret_inflate_end = inflateEnd(&inf_strm);
    gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_deflate;
    (void)ret_deflate_copy;
    (void)ret_deflate_tune;
    (void)ret_gzwrite;
    (void)ret_inflate_init;
    (void)ret_inflate;
    (void)ret_deflate_end_src;
    (void)ret_deflate_end_copy;
    (void)ret_inflate_end;
    (void)ver;
    (void)def_strm.total_out;

    return 66;
    // API sequence test completed successfully
}