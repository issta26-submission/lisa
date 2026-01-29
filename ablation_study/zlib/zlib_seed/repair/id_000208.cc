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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_strm;
    z_stream inflateback_strm;
    Bytef input_buf[] = "zlib API sequence payload to compress, write via gzFile opened with gzopen64, rewind, and cleanup.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef comp_buf[65536];
    unsigned char window[1 << 15];
    const char *ver;
    gzFile gz;
    uLong bound;
    uLong compressed_len;
    unsigned int out_avail;
    int ret_deflate_init;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_inflateback_init;
    int ret_inflateback_end;
    int ret_gz_write;
    int ret_gzrewind;
    int ret_gz_close;

    // step 2: Setup / Initialize
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    memset(&inflateback_strm, 0, sizeof(inflateback_strm));
    memset(comp_buf, 0, sizeof(comp_buf));
    memset(window, 0, sizeof(window));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&deflate_strm, input_len);

    // step 3: Operate - compress and prepare inflate-back context
    deflate_strm.next_in = input_buf;
    deflate_strm.avail_in = (uInt)input_len;
    deflate_strm.next_out = comp_buf;
    out_avail = (unsigned int)((bound <= (uLong)sizeof(comp_buf)) ? bound : (uLong)sizeof(comp_buf));
    deflate_strm.avail_out = (uInt)out_avail;
    ret_deflate_call = deflate(&deflate_strm, 4);
    compressed_len = deflate_strm.total_out;
    ret_inflateback_init = inflateBackInit_(&inflateback_strm, 15, window, ver, (int)sizeof(z_stream));

    // step 4: Validate / Write / Cleanup
    gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)comp_buf, (unsigned int)compressed_len);
    ret_gzrewind = gzrewind(gz);
    ret_gz_close = gzclose(gz);
    ret_inflateback_end = inflateBackEnd(&inflateback_strm);
    ret_deflate_end = deflateEnd(&deflate_strm);
    (void)ret_deflate_init;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_inflateback_init;
    (void)ret_inflateback_end;
    (void)ret_gz_write;
    (void)ret_gzrewind;
    (void)ret_gz_close;
    (void)ver;
    (void)bound;
    (void)out_avail;
    (void)compressed_len;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}