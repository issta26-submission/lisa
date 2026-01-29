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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib API sequence: compress this payload, write via gzopen64, rewind and rewrite, then cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[65536];
    const char *ver;
    gzFile gz;
    uLong bound;
    int ret_deflate_init;
    int ret_deflate_call;
    uLong compressed_len;
    int ret_gz_write1;
    int ret_gzrewind;
    int ret_gz_write2;
    int ret_inflateBackEnd;
    int ret_deflate_end;
    int ret_gz_close;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&strm, input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)((bound <= (uLong)sizeof(comp_buf)) ? bound : (uLong)sizeof(comp_buf));

    // step 3: Core operations
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gz_write1 = gzwrite(gz, (voidpc)comp_buf, (unsigned int)compressed_len);
    ret_gzrewind = gzrewind(gz);
    ret_gz_write2 = gzwrite(gz, (voidpc)comp_buf, (unsigned int)compressed_len);

    // step 4: Cleanup and validation
    ret_inflateBackEnd = inflateBackEnd(&strm);
    ret_deflate_end = deflateEnd(&strm);
    ret_gz_close = gzclose(gz);
    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_gz_write1;
    (void)ret_gzrewind;
    (void)ret_gz_write2;
    (void)ret_inflateBackEnd;
    (void)ret_deflate_end;
    (void)ret_gz_close;
    (void)ver;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}