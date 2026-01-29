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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    Bytef input_buf[] = "zlib: example payload to compress, compute crc32, write to gz and seek using both gzseek and gzseek64.";
    uInt input_len = (uInt)(sizeof(input_buf) - 1);
    Bytef outbuf[65536];
    uLong bound;
    const char *ver;
    gzFile gz;
    int ret_deflate_init;
    int ret_deflate_params;
    int ret_deflate_call;
    int ret_deflate_end;
    int ret_gz_write;
    int ret_gz_close;
    off_t off_small;
    off64_t off_large;
    uLong compressed_len;
    uLong crc;

    // step 2: Setup
    memset(&strm, 0, sizeof(strm));
    memset(outbuf, 0, sizeof(outbuf));
    ver = zlibVersion();
    ret_deflate_init = deflateInit_(&strm, 6, ver, (int)sizeof(z_stream));
    ret_deflate_params = deflateParams(&strm, 1, 0);

    // step 3: Core operations
    bound = deflateBound(&strm, (uLong)input_len);
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = outbuf;
    strm.avail_out = (uInt)((bound <= (uLong)sizeof(outbuf)) ? bound : (uLong)sizeof(outbuf));
    ret_deflate_call = deflate(&strm, 4);
    compressed_len = strm.total_out;
    crc = crc32(0L, input_buf, input_len);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    ret_gz_write = gzwrite(gz, (voidpc)outbuf, (unsigned int)compressed_len);
    off_small = gzseek(gz, (off_t)0, 0);
    off_large = gzseek64(gz, (off64_t)0, 0);
    ret_gz_close = gzclose(gz);

    // step 4: Cleanup
    ret_deflate_end = deflateEnd(&strm);
    (void)ret_deflate_init;
    (void)ret_deflate_params;
    (void)ret_deflate_call;
    (void)ret_deflate_end;
    (void)ret_gz_write;
    (void)ret_gz_close;
    (void)off_small;
    (void)off_large;
    (void)compressed_len;
    (void)crc;
    (void)ver;
    (void)input_len;
    // API sequence test completed successfully
    return 66;
}