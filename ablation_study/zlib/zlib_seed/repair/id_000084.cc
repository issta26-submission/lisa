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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_strm;
    Bytef input_buf[] = "Example input data for zlib deflateTune + gzopen64/gzwrite test.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[2048];
    gzFile gzf;
    const char *ver;
    int ret_deflate_init;
    int ret_deflate;
    int ret_deflate_tune;
    int ret_deflate_end;
    int ret_gzwrite;

    // step 2: Setup (initialize stream and obtain zlib version, open gz file)
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));
    deflate_strm.next_in = input_buf;
    deflate_strm.avail_in = (uInt)input_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)sizeof(comp_buf);
    gzf = gzopen64("test_zlib_api_sequence.tmp.gz", "wb");

    // step 3: Core operations (deflate, write compressed data, tune deflate parameters)
    ret_deflate = deflate(&deflate_strm, 0);
    ret_gzwrite = gzwrite(gzf, comp_buf, (unsigned int)deflate_strm.total_out);
    ret_deflate_tune = deflateTune(&deflate_strm, 8, 32, 64, 128);

    // step 4: Cleanup (end deflate and close gz file)
    ret_deflate_end = deflateEnd(&deflate_strm);
    gzclose(gzf);

    (void)ret_deflate_init;
    (void)ret_deflate;
    (void)ret_deflate_tune;
    (void)ret_deflate_end;
    (void)ret_gzwrite;
    (void)ver;
    (void)input_len;
    (void)comp_buf;
    (void)deflate_strm.total_out;

    // API sequence test completed successfully
    return 66;
}