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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib integration test: compress, compute crc, write gz, and seek (32/64-bit).";
    uInt input_len = (uInt)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    uInt outbuf_size = (uInt)sizeof(outbuf);
    uLong compressed_len;
    const char * ver;
    gzFile gz;
    off_t off32;
    off64_t off64;
    uLong crc;
    int ret_deflate_init;
    int ret_deflate_params;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_close;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    strm.next_in = input_buf;
    strm.avail_in = input_len;
    strm.next_out = outbuf;
    strm.avail_out = outbuf_size;

    // step 3: Configure and Operate
    ret_deflate_params = deflateParams(&strm, 9, 0);
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    crc = crc32(0UL, outbuf, (uInt)compressed_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, outbuf, (unsigned int)compressed_len);
    off32 = gzseek(gz, (off_t)0, 1);
    off64 = gzseek64(gz, (off64_t)0, 1);

    // step 4: Validate and Cleanup
    ret_gz_close = gzclose(gz);
    ret_deflate_end = deflateEnd(&strm);
    (void)ret_deflate_init;
    (void)ret_deflate_params;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ver;
    (void)compressed_len;
    (void)crc;
    (void)off32;
    (void)off64;
    // API sequence test completed successfully
    return 66;
}