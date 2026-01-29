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
//<ID> 183
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib: example payload for deflate, write, seek and crc32 validation.";
    uInt input_len = (uInt)(sizeof(input_buf) - 1);
    Bytef outbuf[262144];
    uLong compressed_len;
    uLong bound;
    const char * ver;
    gzFile gz;
    off_t off1;
    off64_t off2;
    uLong crc;
    int ret_deflate_init;
    int ret_deflate_params;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_close;
    int ret_gz_flush;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));

    // step 3: Configure and Operate
    ret_deflate_params = deflateParams(&strm, 9, 0);
    bound = deflateBound(&strm, (uLong)input_len);
    strm.next_in = input_buf;
    strm.avail_in = input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, outbuf, (unsigned int)compressed_len);
    ret_gz_flush = gzflush(gz, 2);
    off1 = gzseek(gz, (off_t)0, 0);
    off2 = gzseek64(gz, (off64_t)0, 0);
    ret_gz_close = gzclose(gz);

    // step 4: Validation and Cleanup
    ret_deflate_end = deflateEnd(&strm);
    crc = crc32(0L, input_buf, input_len);
    (void)ret_deflate_init;
    (void)ret_deflate_params;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_flush;
    (void)ret_gz_close;
    (void)off1;
    (void)off2;
    (void)bound;
    (void)ver;
    (void)compressed_len;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}