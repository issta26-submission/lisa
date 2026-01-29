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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib: example payload for deflate, gzwrite, seek, and crc32 validation.";
    uLong input_len = (uLong)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
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

    // step 2: Setup / Initialize
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_params = deflateParams(&strm, 9, 0);

    // step 3: Operate / Validate
    strm.next_in = (Bytef *)input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)sizeof(outbuf);
    ret_deflate_call = deflate(&strm, 4);
    uLong compressed_len = strm.total_out;
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    off32 = gzseek(gz, (off_t)0, 0);
    off64 = gzseek64(gz, (off64_t)0, 0);
    crc = crc32(0UL, input_buf, (uInt)input_len);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    (void)ret_deflate_init;
    (void)ret_deflate_params;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)ver;
    (void)compressed_len;
    (void)off32;
    (void)off64;
    (void)crc;
    // API sequence test completed successfully
    return 66;
}