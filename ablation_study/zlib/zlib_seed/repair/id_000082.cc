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
//<ID> 82
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
    Bytef input_buf[] = "Sample data for zlib deflate and gzwrite testing.";
    Bytef comp_buf[1024];
    gzFile gzf;
    const char *ver;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_copy;
    int ret_deflate_tune;
    int ret_deflate_end;
    int ret_deflate_end_copy;
    int ret_gzwrite;

    // step 2: Setup (initialize streams and obtain zlib version)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&deflate_copy_strm, 0, sizeof(deflate_copy_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));
    deflate_strm.next_in = input_buf;
    deflate_strm.avail_in = (uInt)(sizeof(input_buf) - 1);
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Core operations (compress, copy/tune state, write compressed data)
    ret_deflate = deflate(&deflate_strm, 0);
    ret_deflate_copy = deflateCopy(&deflate_copy_strm, &deflate_strm);
    ret_deflate_tune = deflateTune(&deflate_copy_strm, 4, 8, 16, 32);
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)deflate_strm.total_out);

    // step 4: Cleanup (end streams and close file)
    ret_deflate_end = deflateEnd(&deflate_strm);
    ret_deflate_end_copy = deflateEnd(&deflate_copy_strm);
    gzclose(gzf);

    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate;
    (void)ret_deflate_copy;
    (void)ret_deflate_tune;
    (void)ret_gzwrite;
    (void)ret_deflate_end;
    (void)ret_deflate_end_copy;
    (void)deflate_strm.total_out;

    // API sequence test completed successfully
    return 66;
}