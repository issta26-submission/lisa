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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_strm;
    Bytef input_buf[] = "Example data to compress using zlib for API sequencing test.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[1024];
    const char * ver;
    gzFile gzf;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_tune;
    int ret_deflate_end;
    int ret_gzwrite;

    // step 2: Setup (initialize stream and obtain zlib version)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));
    deflate_strm.next_in = input_buf;
    deflate_strm.avail_in = (uInt)input_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);

    // step 3: Core operations (compress, tune deflate, write compressed data to file)
    ret_deflate = deflate(&deflate_strm, 0);
    ret_deflate_tune = deflateTune(&deflate_strm, 4, 16, 64, 256);
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)deflate_strm.total_out);

    // step 4: Cleanup (end stream and close file)
    ret_deflate_end = deflateEnd(&deflate_strm);
    gzclose(gzf);

    (void)ver;
    (void)ret_deflate_init;
    (void)ret_deflate;
    (void)ret_deflate_tune;
    (void)ret_gzwrite;
    (void)ret_deflate_end;
    (void)input_len;
    (void)deflate_strm.total_out;

    // API sequence test completed successfully
    return 66;
}